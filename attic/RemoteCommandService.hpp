#if !defined(SMILE_REMOTECOMMANDSERVICE_HPP__)
#define SMILE_REMOTECOMMANDSERVICE_HPP__

#include "HandshakeService.hpp"
#include "RemoteCommandExchangeAttributesReply.hpp"

namespace smile
{

class RemoteCommandService : public HandshakeService
{
public:
    static const uint16_t IDENTIFIER = 0xe008;

    RemoteCommandService(AS400Impl& as400);

    virtual void disconnect();
    int getCCSID() const;
    uint16_t getDataStreamLevel() const;
    virtual uint16_t getIdentifier() const;
    virtual const char* getName() const;
    void imbueAttributes(RemoteCommandExchangeAttributesReply& reply);

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet);

private:
    int m_ccsid;
    uint16_t m_dataStreamLevel;
};

inline int RemoteCommandService::getCCSID() const
{
    return m_ccsid;
}

inline uint16_t RemoteCommandService::getDataStreamLevel() const
{
    return m_dataStreamLevel;
}

}

#endif
