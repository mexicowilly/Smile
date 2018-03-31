#if !defined(SMILE_SIGNONSERVICE_HPP__)
#define SMILE_SIGNONSERVICE_HPP__

#include "AsynchronousHostService.hpp"

namespace smile
{

class SignonService : public AsynchronousHostService
{
public:
    static const uint16_t IDENTIFIER = 0xe009;

    SignonService(AS400Impl& as400);

    virtual void disconnect();
    virtual uint16_t getIdentifier() const;
    virtual const char* getName() const;

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet);
};

inline SignonService::SignonService(AS400Impl& as400)
    : AsynchronousHostService(as400)
{
}

}

#endif
