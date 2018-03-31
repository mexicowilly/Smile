#include "ExchangeRandomSeedsRequest.hpp"
#include <openssl/rand.h>

namespace smile
{

ExchangeRandomSeedsRequest::ExchangeRandomSeedsRequest(uint16_t serverId)
    : PacketRequest(28)
{
    m_packet.setRequestReplyId(0x7001);
    m_packet.setServerId(serverId);
    m_packet.setTemplateLength(8);
    m_packet.setInt8(4, 1);
    uint8_t bytes[8];
    RAND_bytes(bytes, 8);
    while (bytes[0] >= 0xe0)
        RAND_bytes(bytes, 1);
    m_packet.setBytes(20, bytes, 8);
}

const char* ExchangeRandomSeedsRequest::getName() const
{
    return "exchange random seeds";
}

}
