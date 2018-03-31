#include "ServicePool.hpp"
#include "LogUtil.hpp"
#include <smile/Exception.hpp>
#include <smile/System.hpp>
#include <smile/Notifier.hpp>
#include "Locker.hpp"
#include "StaticInitializer.hpp"
#include <errno.h>

namespace
{

const smile::Text CAPACITY_PROPERTY(SMILE_WSTR("service.pool.capacity"));
const smile::Text INACTIVITY_TIMEOUT_PROPERTY(SMILE_WSTR("service.pool.inactivity.timeout"));
const smile::Text ENABLED_PROPERTY(SMILE_WSTR("service.pool.enabled"));
const smile::Text AT(SMILE_WSTR("@"));
const smile::Text WAIT_TIMEOUT_PROPERTY(SMILE_WSTR("service.pool.wait.timeout"));

inline bool before(const struct timeval& first, const struct timeval& second)
{
    return first.tv_sec == second.tv_sec ?
        first.tv_usec < second.tv_usec :
        first.tv_sec < second.tv_sec;
}

smile::Text nodeKey(smile::AS400& as400)
{
    return as400.getUserName() + AT + as400.getSystemName();
}

}

namespace smile
{

ServicePool::ServicePool()
{
    Notifier::getDefault().addObserver(*this,
                                       &ServicePool::propertyChanged,
                                       Properties::CHANGE_NOTIFICATION,
                                       &System::getInstance().getProperties());
    pthread_mutex_init(&m_mutex, 0);
    pthread_cond_init(&m_condition, 0);
}

ServicePool::~ServicePool()
{
    Notifier::getDefault().removeObserver(*this);
    clear();
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_condition);
}

void ServicePool::checkIn(HostService* service)
{
    if (service == 0)
        return;
    Logger& log = LogUtil::smileLogger();
    if (System::getInstance().getProperties().getBool(ENABLED_PROPERTY))
    {
        Text key = nodeKey(service->getAS400());
        SMILE_LOG_DEBUG(log, "Checking in " << key << ":" << service->getName());
        Locker locker(m_mutex);
        std::map<Text, ServiceMap>::iterator foundAS400 = m_services.find(key);
        if (foundAS400 == m_services.end())
        {
            SMILE_LOG_WARN(log, key << " is not a controlled system. Just deleting " <<
                service->getName());
            delete service;
        }
        else
        {
            ServiceMap::iterator foundNode = foundAS400->second.find(service->getIdentifier());
            if (foundNode == foundAS400->second.end())
            {
                SMILE_LOG_WARN(log, key << ":" << service->getName() <<
                    " is not a controlled system. Just deleting " <<
                    service->getName());
                delete service;
            }
            else
            {
                try
                {
                    foundNode->second.checkIn(service);
                }
                catch (smile::Exception& se)
                {
                    SMILE_LOG_ERROR(log, "Exception occurred while checking in " <<
                        key << ":" << service->getName() << ": " << se);
                    delete service;
                }
            }
        }
    }
    else
    {
        SMILE_LOG_WARN(log, "Service pool not active. Just deleting " <<
            nodeKey(service->getAS400()) << ":" << service->getName());
        delete service;
    }
}

HostService* ServicePool::checkOut(ServiceFactory& factory)
{
    Logger& log = LogUtil::smileLogger();
    Text key = nodeKey(factory.getAS400());
    HostService* service = 0;
    if (System::getInstance().getProperties().getBool(ENABLED_PROPERTY))
    {
        SMILE_LOG_DEBUG(log, "Checking out service " << key << ":" <<
            std::hex << std::showbase << factory.getServiceIdentifier());
        Locker locker(m_mutex);
        service = m_services[key][factory.getServiceIdentifier()].checkOut(factory);
    }
    else
    {
        SMILE_LOG_DEBUG(log, "No service pool enabled. Just creating " <<
            key << ":" <<
            std::hex << std::showbase << factory.getServiceIdentifier());
        service = factory.createHostService();
    }
    return service;
}

ServicePool& ServicePool::getInstance()
{
    return StaticInitializer::getInstance().getServicePool();
}

void ServicePool::propertyChanged(const Notification& notification)
{
    const Text& property = notification.getData().get(Properties::PROPERTY_NAME);
    SMILE_LOG_DEBUG(LogUtil::smileLogger(),
        "Service pool notes that system property " << property <<
        " has changed");
    if (property == ENABLED_PROPERTY)
    {
        if (!notification.getData().getBool(Properties::NEW_VALUE))
        {
            Locker locker(m_mutex);
            clear();
        }
    }
    else if (property == CAPACITY_PROPERTY)
    {
        Locker locker(m_mutex);
        setCapacity(notification.getData().getInt(Properties::NEW_VALUE));
    }
}

ServicePool::HostNode::HostNode(HostService* service)
    : m_hostService(service), m_thread(pthread_self())
{
    memset(&m_expiration, 0, sizeof(m_expiration));
}

ServicePool::HostNode::HostNode(HostService* service, struct timeval& expiration)
    : m_hostService(service),
      m_expiration(expiration),
      m_thread(pthread_self())
{
}

ServicePool::HostNode::HostNode(const HostNode& node)
    : m_hostService(node.m_hostService),
      m_expiration(node.m_expiration),
      m_thread(node.m_thread)
{
}

ServicePool::HostNode& ServicePool::HostNode::operator= (const HostNode& node)
{
    if (&node != this)
    {
        m_hostService = node.m_hostService;
        m_expiration = node.m_expiration;
        m_thread = node.m_thread;
    }
    return *this;
}

bool ServicePool::HostNode::operator< (const HostNode& node) const
{
    return m_hostService < node.m_hostService;
}

ServicePool::ServiceNode::ServiceNode()
{
}

ServicePool::ServiceNode::ServiceNode(const ServiceNode& node)
    : m_available(node.m_available),
      m_unavailable(node.m_unavailable)
{
}

ServicePool::ServiceNode& ServicePool::ServiceNode::operator= (const ServiceNode& node)
{
    if (&node != this)
    {
        m_available = node.m_available;
        m_unavailable = node.m_unavailable;
    }
    return *this;
}

void ServicePool::ServiceNode::checkIn(HostService* service)
{
    bool broadcastNeeded = getTotal() == System::getInstance().
        getProperties().getInt(CAPACITY_PROPERTY);
    if (m_unavailable.erase(HostNode(service)) == 0)
    {
        SMILE_LOG_WARN(LogUtil::smileLogger(),
            "Just deleting uncontrolled " << service->getName() << " service");
        delete service;
    }
    else
    {
        struct timeval timeVal;
        if (gettimeofday(&timeVal, 0) != 0)
        {
            delete service;
            throw ErrnoException(__FILE__, __LINE__, "Could not get time of day", errno);
        }
        timeVal.tv_sec += System::getInstance().getProperties().getInt(INACTIVITY_TIMEOUT_PROPERTY);
        m_available.push_back(HostNode(service, timeVal));
        assert(getTotal() <= System::getInstance().getProperties().getInt(CAPACITY_PROPERTY));
        if (broadcastNeeded)
            pthread_cond_broadcast(&ServicePool::getInstance().m_condition);
    }
}

HostService* ServicePool::ServiceNode::checkOut(ServiceFactory& factory)
{
    HostService* found = 0;
    struct timeval now;
    if (gettimeofday(&now, 0) != 0)
        throw ErrnoException(__FILE__, __LINE__, "Could not get time of day", errno);
    Logger& log = LogUtil::smileLogger();
    while (found == 0)
    {
        while (found == 0 && !m_available.empty())
        {
            HostNode node = m_available.back();
            m_available.pop_back();
            if (before(node.getExpiration(), now))
            {
                delete node.getHostService();
            }
            else
            {
                found = node.getHostService();
                m_unavailable.insert(HostNode(found));
            }
        }
        if (found == 0)
        {
            if (getTotal() >= System::getInstance().getProperties().getInt(CAPACITY_PROPERTY))
            {
                std::set<HostNode>::iterator notThisThread = std::find_if(m_unavailable.begin(),
                                                                          m_unavailable.end(),
                                                                          NotThreadEqual());
                if (notThisThread == m_unavailable.end())
                {
                    log.info("Single-thread service pool overflow. Creating an uncontrolled service.");
                    found = factory.createHostService();
                }
                else
                {
                    log.debug("Waiting in line for available service...");
                    struct timespec timeSpec;
                    timeSpec.tv_sec = now.tv_sec + System::getInstance().getProperties().getInt(WAIT_TIMEOUT_PROPERTY);
                    timeSpec.tv_nsec = now.tv_usec * 1000;
                    int rc = pthread_cond_timedwait(&ServicePool::getInstance().m_condition,
                                                    &ServicePool::getInstance().m_mutex,
                                                    &timeSpec);
                    if (rc == ETIMEDOUT)
                    {
                        log.info("Multi-thread service pool wait timed out. Creating an uncontrolled service.");
                        found = factory.createHostService();
                    }
                }
            }
            else
            {
                found = factory.createHostService();
                m_unavailable.insert(HostNode(found));
            }
        }
    }
    return found;
}

void ServicePool::ServiceNode::clear()
{
    std::for_each(m_available.begin(), m_available.end(), DeleteHostService());
    m_available.clear();
    m_unavailable.clear();
}

void ServicePool::ServiceNode::setCapacity(unsigned capacity)
{
    int desired = static_cast<int>(getTotal()) - static_cast<int>(capacity);
    if (desired > 0)
    {
        std::vector<HostNode>::size_type toDelete =
            std::min(static_cast<std::vector<HostNode>::size_type>(desired), m_available.size());
        std::vector<HostNode>::reverse_iterator lower = m_available.rbegin();
        std::vector<HostNode>::reverse_iterator upper = lower + toDelete;
        std::for_each(lower, upper, DeleteHostService());
        m_available.resize(m_available.size() - toDelete, HostNode(0));
    }
}

}
