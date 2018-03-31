#include "CentralService.hpp"
#include "NlsExchangeAttributesReply.hpp"
#include "EndJobRequest.hpp"

namespace smile
{

CentralService::CentralService(AS400Impl& as400)
    : HandshakeService(as400), m_ccsid(0)
{
}

PacketReply* CentralService::convertPacketToReply(const AccessPacket& packet)
{
    PacketReply* result;
    switch (packet.getRequestReplyId())
    {
    case 0x1301:
        result = new NlsExchangeAttributesReply(packet);
        break;
    default:
        result = HandshakeService::convertPacketToReply(packet);
        break;
    }
    return result;
}

void CentralService::disconnect()
{
    EndJobRequest request(IDENTIFIER);
    send(request);
    AsynchronousHostService::disconnect();
}

uint16_t CentralService::getIdentifier() const
{
    return IDENTIFIER;
}

const char* CentralService::getName() const
{
    return "as-central";
}

void CentralService::imbueAttributes(NlsExchangeAttributesReply& reply)
{
    m_ccsid = reply.getCCSID();
}

}
