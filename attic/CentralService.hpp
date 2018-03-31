#if !defined(SMILE_CENTRALSERVICE_HPP__)
#define SMILE_CENTRALSERVICE_HPP__

#include "HandshakeService.hpp"
#include "NlsExchangeAttributesReply.hpp"

namespace smile
{

class CentralService : public HandshakeService
{
public:
    static const uint16_t IDENTIFIER = 0xe000;

    CentralService(AS400Impl& as400);

    virtual void disconnect();
    int getCCSID() const;
    virtual uint16_t getIdentifier() const;
    virtual const char* getName() const;
    void imbueAttributes(NlsExchangeAttributesReply& reply);

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet);

private:
    int m_ccsid;
};

inline int CentralService::getCCSID() const
{
    return m_ccsid;
}

}

#endif
