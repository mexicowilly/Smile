#include "HandshakeService.hpp"
#include "ExchangeRandomSeedsRequest.hpp"
#include "ExchangeRandomSeedsReply.hpp"
#include "SignonTextConverter.hpp"
#include "StartServerRequest.hpp"
#include "StartServerReply.hpp"

namespace smile
{

void HandshakeService::connect()
{
    m_as400.prepareConnection();
    AsynchronousHostService::connect();
    ExchangeRandomSeedsRequest exchangeRequest = ExchangeRandomSeedsRequest(getIdentifier());
    ExchangeRandomSeedsReply* exchangeReply = sendAndReceive<ExchangeRandomSeedsReply>(exchangeRequest);
    std::auto_ptr<ExchangeRandomSeedsReply> autoExchangeReply(exchangeReply);
    if (exchangeReply->getReturnCode() != 0)
    {
        throw HostServiceException(__FILE__, __LINE__,
                                   "Error exchanging random seeds", exchangeReply->getReturnCode());
    }
    std::vector<uint8_t> pwd = m_as400.getEncryptedPassword(exchangeRequest.getClientSeed(),
                                                            exchangeReply->getServerSeed());
    std::vector<uint8_t> user = SignonTextConverter::getInstance().
        convertToSignonEbcdic(m_as400.getUserName());
    user.resize(10, 0x40);
    StartServerRequest startRequest = StartServerRequest(getIdentifier(),
                                                         user,
                                                         pwd,
                                                         m_as400.getAuthentication());
    StartServerReply* startReply = sendAndReceive<StartServerReply>(startRequest);
    std::auto_ptr<StartServerReply> autoStartReply(startReply);
    if (startReply->getReturnCode() != 0)
    {
        throw HostServiceException(__FILE__, __LINE__,
                                   "Could not start server service", startReply->getReturnCode());
    }
}

PacketReply* HandshakeService::convertPacketToReply(const AccessPacket& packet)
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
    default:
        result = new PacketReply(packet);
        break;
    }
    return result;
}

}
