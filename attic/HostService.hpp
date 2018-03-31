#if !defined(SMILE_HOSTSERVICE_HPP__)
#define SMILE_HOSTSERVICE_HPP__

#include <inttypes.h>
#include <pthread.h>
#include <set>
#include <sstream>
#include "Connection.hpp"
#include "PacketRequest.hpp"
#include "PacketReply.hpp"
#include "AS400Impl.hpp"
#include "TypeInfo.hpp"
#include <smile/Exception.hpp>

namespace smile
{

class HostService
{
public:
    HostService(AS400Impl& as400);
    virtual ~HostService();

    virtual void connect();
    virtual void disconnect();
    AS400& getAS400();
    virtual const char* getName() const = 0;
    virtual uint16_t getIdentifier() const = 0;
    virtual bool isConnected() const;
    virtual PacketReply* receive(uint32_t correlation) = 0;
    virtual uint32_t send(PacketRequest& request) = 0;
    template <class T>
    T* sendAndReceive(PacketRequest& request);
    virtual void sendIgnoringReply(PacketRequest& request);

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet) = 0;
    uint32_t nextCorrelationId();

    AS400Impl& m_as400;
    Connection m_connection;
    std::set<uint32_t> m_discarded;
    pthread_mutex_t m_discardMutex;

private:
    pthread_mutex_t m_correlationMutex;
    uint32_t m_lastCorrelation;
};

inline AS400& HostService::getAS400()
{
    return m_as400.getAS400();
}

template <class T>
T* HostService::sendAndReceive(PacketRequest& request)
{
    PacketReply* rawReply = receive(send(request));
    T* reply = dynamic_cast<T*>(rawReply);
    if (reply == 0)
    {
        std::ostringstream stream;
        stream << "Unexpected reply type: " << TypeInfo::getName(typeid(*rawReply));
        delete rawReply;
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
    return reply;
}

}

#endif
