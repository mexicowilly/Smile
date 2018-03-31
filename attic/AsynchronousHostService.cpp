#include "AsynchronousHostService.hpp"
#include "ByteOrder.hpp"
#include "LogUtil.hpp"
#include <smile/System.hpp>
#include <errno.h>
#include <signal.h>

namespace smile
{

AsynchronousHostService::AsynchronousHostService(AS400Impl& as400)
    : HostService(as400),
      m_threadException(0),
      m_threadInitialized(false)
{
    pthread_mutex_init(&m_conditionMutex, 0);
    pthread_cond_init(&m_threadCondition, 0);
    m_pollTimeOut = System::getInstance().getProperties().
        getInt("service.thread.poll.timeout");
}

AsynchronousHostService::~AsynchronousHostService()
{
    disconnect();
    pthread_mutex_destroy(&m_conditionMutex);
    pthread_cond_destroy(&m_threadCondition);
    delete m_threadException;
    Logger& log = LogUtil::smileLogger();
    for (ReplyMap::iterator itor = m_replyMap.begin();
         itor != m_replyMap.end();
         itor++)
    {
        std::queue<PacketReply*>& q = (*itor).second;
        if (!q.empty())
        {
            SMILE_LOG_DEBUG(log, "Deleting " << q.size() <<
                "uncontrolled replies for correlation " << (*itor).first);
            while (!q.empty())
            {
                delete q.front();
                q.pop();
            }
        }
    }
}

void AsynchronousHostService::connect()
{
    HostService::connect();
    if (pthread_create(&m_thread, 0, threadMain, this) != 0)
    {
        HostService::disconnect();
        throw ErrnoException(__FILE__, __LINE__, "Could not create thread", errno);
    }
    m_threadInitialized = true;
}

void AsynchronousHostService::disconnect()
{
    if (m_threadInitialized)
    {
        pthread_cancel(m_thread);
        pthread_join(m_thread, 0);
    }
    HostService::disconnect();
}

bool AsynchronousHostService::isConnected() const
{
    return HostService::isConnected() &&
           m_threadException == 0 &&
           m_threadInitialized &&
           pthread_kill(m_thread, 0) == 0;
}

PacketReply* AsynchronousHostService::receive(uint32_t correlation)
{
    while (true)
    {
        pthread_mutex_lock(&m_conditionMutex);
        if (!m_replyMap[correlation].empty())
        {
            PacketReply* reply = m_replyMap[correlation].front();
            m_replyMap[correlation].pop();
            pthread_mutex_unlock(&m_conditionMutex);
            return reply;
        }
        else if (m_threadException != 0)
        {
            pthread_mutex_unlock(&m_conditionMutex);
            throw *m_threadException;
        }
        pthread_cond_wait(&m_threadCondition, &m_conditionMutex);
        pthread_mutex_unlock(&m_conditionMutex);
    }
}

uint32_t AsynchronousHostService::send(PacketRequest& request)
{
    if (m_threadException != 0)
        throw *m_threadException;
    uint32_t correlation = nextCorrelationId();
    request.setCorrelation(correlation);
    request.send(m_connection);
    return correlation;
}

void* AsynchronousHostService::threadMain(void* arg)
{
    Logger& log = LogUtil::smileLogger();
    AsynchronousHostService* service = reinterpret_cast<AsynchronousHostService*>(arg);
    uint32_t size;
    while (service->m_threadException == 0)
    {
        try
        {
            pthread_testcancel();
            bool hasInput = service->m_connection.waitForInput(service->m_pollTimeOut);
            pthread_testcancel();
            if (hasInput)
            {
                service->m_connection.read(reinterpret_cast<uint8_t*>(&size),
                                           sizeof(uint32_t));
                size = N32_TO_H(size);
                AccessPacket packet(size);
                packet.read(service->m_connection);
                SMILE_LOG_DEBUG(log, "Read a packet of " << size << " bytes");
                if (packet.getInt8(6) != 0xe0)
                    throw InvalidStateException(__FILE__, __LINE__, "Invalid packet received");
                pthread_mutex_lock(&service->m_discardMutex);
                std::set<uint32_t>::size_type erased =
                    service->m_discarded.erase(packet.getCorrelation());
                pthread_mutex_unlock(&service->m_discardMutex);
                if (erased > 0)
                {
                    SMILE_LOG_DEBUG(log, "The packet was discardable");
                    continue;
                }
                PacketReply* reply = service->convertPacketToReply(packet);
                SMILE_LOG_DEBUG(log, "Received and converted " <<
                    reply->getName() << " reply");
                pthread_mutex_lock(&service->m_conditionMutex);
                service->m_replyMap[reply->getCorrelation()].push(reply);
                pthread_cond_broadcast(&service->m_threadCondition);
                pthread_mutex_unlock(&service->m_conditionMutex);
            }
        }
        catch (Exception& except)
        {
            pthread_testcancel();
            SMILE_LOG_DEBUG(log, "Got exception " << except);
            pthread_mutex_lock(&service->m_conditionMutex);
            if (service->m_threadException == 0)
                service->m_threadException = except.duplicate();
            pthread_cond_broadcast(&service->m_threadCondition);
            pthread_mutex_unlock(&service->m_conditionMutex);
        }
    }
    return 0;
}

}
