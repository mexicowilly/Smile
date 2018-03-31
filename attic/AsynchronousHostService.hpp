#if !defined(SMILE_ASYNCHRONOUSHOSTSERVICE_HPP__)
#define SMILE_ASYNCHRONOUSHOSTSERVICE_HPP__

#include "HostService.hpp"
#include <map>
#include <queue>

namespace smile
{

class Exception;

class AsynchronousHostService : public HostService
{
public:
    AsynchronousHostService(AS400Impl& as400);
    ~AsynchronousHostService();

    virtual void connect();
    virtual void disconnect();
    virtual bool isConnected() const;
    virtual PacketReply* receive(uint32_t correlation);
    virtual uint32_t send(PacketRequest& request);

private:
    typedef std::map<uint32_t, std::queue<PacketReply*> > ReplyMap;

    static void* threadMain(void* arg);

    Exception* m_threadException;
    pthread_cond_t m_threadCondition;
    pthread_mutex_t m_conditionMutex;
    pthread_t m_thread;
    ReplyMap m_replyMap;
    bool m_threadInitialized;
    int64_t m_pollTimeOut;
};

}

#endif
