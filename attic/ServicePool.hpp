#if !defined(SMILE_SERVICEPOOL_HPP__)
#define SMILE_SERVICEPOOL_HPP__

#include "HostService.hpp"
#include "ServiceFactory.hpp"
#include <sys/time.h>
#include <algorithm>

namespace smile
{

class Notification;

class ServicePool
{
public:
    static ServicePool& getInstance();

    void checkIn(HostService* service);
    HostService* checkOut(ServiceFactory& factory);

protected:
    friend class StaticInitializer;

    ServicePool();
    ~ServicePool();

private:
    class HostNode
    {
    public:
        HostNode(HostService* service);
        HostNode(HostService* service, struct timeval& expiration);
        HostNode(const HostNode& node);

        HostNode& operator= (const HostNode& node);
        bool operator< (const HostNode& node) const;

        HostService* getHostService();
        struct timeval& getExpiration();
        pthread_t getThread();

    private:
        HostService* m_hostService;
        struct timeval m_expiration;
        pthread_t m_thread;
    };

    class ServiceNode
    {
    public:
        ServiceNode();
        ServiceNode(const ServiceNode& node);
        ServiceNode& operator= (const ServiceNode& node);

        void checkIn(HostService* service);
        HostService* checkOut(ServiceFactory& factory);
        void clear();
        unsigned getTotal() const;
        void setCapacity(unsigned capacity);

    private:
        class DeleteHostService : public std::unary_function<HostService, void>
        {
        public:
            void operator() (HostNode& service) const;
        };

        class NotThreadEqual : public std::unary_function<HostNode, bool>
        {
        public:
            NotThreadEqual();

            bool operator() (const HostNode& node) const;

        private:
            pthread_t m_thread;
        };

        std::vector<HostNode> m_available;
        std::set<HostNode> m_unavailable;
    };

    typedef std::map<uint16_t, ServiceNode> ServiceMap;

    class ClearServiceNode : public std::unary_function<ServiceNode, void>
    {
    public:
        void operator() (ServiceNode& node) const;
    };

    class SetServiceNodeCapacity : public std::unary_function<ServiceNode, void>
    {
    public:
        SetServiceNodeCapacity(unsigned capacity);

        void operator() (ServiceNode& node) const;

    private:
        unsigned m_capacity;
    };

    template <class Function>
    class ExecuteMapIterator : public std::unary_function<std::pair<const uint16_t, ServiceNode>, void>
    {
    public:
        ExecuteMapIterator(Function function);

        void operator() (std::pair<const uint16_t, ServiceNode>& p);

    private:
        Function m_function;
    };

    template <class Function>
    class ForEachServiceMap : public std::unary_function<std::pair<const Text, ServiceMap>, void>
    {
    public:
        ForEachServiceMap(Function function);

        void operator() (std::pair<const Text, ServiceMap>& p) const;

    private:
        Function m_function;
    };

    void clear();
    void propertyChanged(const Notification& notification);
    template <class Function>
    void forEachServiceNode(Function function);
    void setCapacity(unsigned capacity);

    pthread_mutex_t m_mutex;
    pthread_cond_t m_condition;
    std::map<Text, ServiceMap> m_services;
};

inline void ServicePool::clear()
{
    forEachServiceNode(ClearServiceNode());
    m_services.clear();
}

template <class Function>
void ServicePool::forEachServiceNode(Function function)
{
    std::for_each(m_services.begin(), m_services.end(), ForEachServiceMap<Function>(function));
}

inline void ServicePool::setCapacity(unsigned capacity)
{
    forEachServiceNode(SetServiceNodeCapacity(capacity));
}

inline struct timeval& ServicePool::HostNode::getExpiration()
{
    return m_expiration;
}

inline HostService* ServicePool::HostNode::getHostService()
{
    return m_hostService;
}

inline pthread_t ServicePool::HostNode::getThread()
{
    return m_thread;
}

inline unsigned ServicePool::ServiceNode::getTotal() const
{
    return m_available.size() + m_unavailable.size();
}

inline void ServicePool::ServiceNode::DeleteHostService::operator() (HostNode& service) const
{
    delete service.getHostService();
}

inline ServicePool::ServiceNode::NotThreadEqual::NotThreadEqual()
    : m_thread(pthread_self())
{
}

inline bool ServicePool::ServiceNode::NotThreadEqual::operator() (const HostNode& node) const
{
    return pthread_equal(m_thread, const_cast<HostNode&>(node).getThread()) == 0;
}

inline void ServicePool::ClearServiceNode::operator() (ServiceNode& node) const
{
    node.clear();
}

template <class Function>
ServicePool::ForEachServiceMap<Function>::ForEachServiceMap(Function function)
    : m_function(function)
{
}

template <class Function>
void ServicePool::ForEachServiceMap<Function>::operator() (std::pair<const Text, ServiceMap>& p) const
{
    std::for_each(p.second.begin(), p.second.end(), ExecuteMapIterator<Function>(m_function));
}

template <class Function>
ServicePool::ExecuteMapIterator<Function>::ExecuteMapIterator(Function function)
    : m_function(function)
{
}

template <class Function>
void ServicePool::ExecuteMapIterator<Function>::operator() (std::pair<const uint16_t, ServiceNode>& p)
{
    m_function(p.second);
}

inline ServicePool::SetServiceNodeCapacity::SetServiceNodeCapacity(unsigned capacity)
    : m_capacity(capacity)
{
}

inline void ServicePool::SetServiceNodeCapacity::operator() (ServiceNode& node) const
{
    node.setCapacity(m_capacity);
}

}

#endif
