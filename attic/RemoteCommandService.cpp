#include "RemoteCommandService.hpp"
#include "RemoteCommandExchangeAttributesReply.hpp"
#include "RunCommandReply.hpp"
#include "CallProgramReply.hpp"
#include "CallProgramFailureReply.hpp"
#include "EndJobRequest.hpp"

namespace smile
{

RemoteCommandService::RemoteCommandService(AS400Impl& as400)
    : HandshakeService(as400), m_ccsid(0), m_dataStreamLevel(0)
{
}

PacketReply* RemoteCommandService::convertPacketToReply(const AccessPacket& packet)
{
    PacketReply* result;
    switch (packet.getRequestReplyId())
    {
    case 0x8001:
        result = new RemoteCommandExchangeAttributesReply(packet);
        break;
    case 0x8002:
        result = new RunCommandReply(packet);
        break;
    case 0x8003:
        result = new CallProgramReply(packet);
        break;
    case 0x9003:
        result = new CallProgramFailureReply(packet);
        break;
    default:
        result = HandshakeService::convertPacketToReply(packet);
        break;
    }
    return result;
}

void RemoteCommandService::disconnect()
{
    EndJobRequest request(IDENTIFIER);
    send(request);
    AsynchronousHostService::disconnect();
}

uint16_t RemoteCommandService::getIdentifier() const
{
    return IDENTIFIER;
}

const char* RemoteCommandService::getName() const
{
    return "as-rmtcmd";
}

void RemoteCommandService::imbueAttributes(RemoteCommandExchangeAttributesReply& reply)
{
    m_ccsid = reply.getCCSID();
    m_dataStreamLevel = reply.getDataStreamLevel();
}

}
