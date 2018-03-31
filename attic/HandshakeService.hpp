#if !defined(SMILE_HANDSHAKESERVICE_HPP__)
#define SMILE_HANDSHAKESERVICE_HPP__

#include "AsynchronousHostService.hpp"

namespace smile
{

class HandshakeService : public AsynchronousHostService
{
public:
    HandshakeService(AS400Impl& as400);

    virtual void connect();

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet);
};

inline HandshakeService::HandshakeService(AS400Impl& as400)
    : AsynchronousHostService(as400)
{
}

}

#endif
