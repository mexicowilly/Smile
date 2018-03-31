#include "SignonService.hpp"
#include "ExchangeRandomSeedsReply.hpp"
#include "StartServerReply.hpp"
#include "SignonExchangeAttributesReply.hpp"
#include "SignonInfoReply.hpp"
#include "EndSignonServiceRequest.hpp"

namespace smile
{

PacketReply* SignonService::convertPacketToReply(const AccessPacket& packet)
{
    PacketReply* result;
    switch (packet.getRequestReplyId())
    {
    case 0xf001:
        result = new ExchangeRandomSeedsReply(packet);
        break;
    case 0xf002:
        result = new StartServerReply(packet);
        break;
    case 0xf003:
        result = new SignonExchangeAttributesReply(packet);
        break;
    case 0xf004:
        result = new SignonInfoReply(packet);
        break;
    case 0xf005:
        // Change password
        assert(false);
        result = 0;
        break;
    case 0xf007:
        // Generate authentication token
        assert(false);
        result = 0;
        break;
    case 0xf008:
        // Signon get authentication token
        assert(false);
        result = 0;
        break;
    default:
        result = new PacketReply(packet);
        break;
    }
    return result;
}

void SignonService::disconnect()
{
    EndSignonServiceRequest request;
    send(request);
    AsynchronousHostService::disconnect();
}

uint16_t SignonService::getIdentifier() const
{
    return IDENTIFIER;
}

const char* SignonService::getName() const
{
    return "as-signon";
}

}
