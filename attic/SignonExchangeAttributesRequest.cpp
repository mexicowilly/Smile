#include "SignonExchangeAttributesRequest.hpp"
#include "SignonService.hpp"
#include <assert.h>

namespace smile
{

SignonExchangeAttributesRequest::SignonExchangeAttributesRequest(const std::vector<uint8_t>& seed)
    : PacketRequest(seed.empty() ? 38 : 52)
{
    m_packet.setServerId(SignonService::IDENTIFIER);
    m_packet.setRequestReplyId(0x7003);
    m_packet.setInt32(20, 10);
    m_packet.setInt16(24, 0x1101);
    m_packet.setInt32(26, 1);
    m_packet.setInt32(30, 8);
    m_packet.setInt16(34, 0x1102);
    m_packet.setInt16(36, 2);
    if (!seed.empty())
    {
        assert(seed.size() == 8);
        m_packet.setInt32(38, 14);
        m_packet.setInt16(42, 0x1103);
        m_packet.setBytes(44, seed);
    }
}

const char* SignonExchangeAttributesRequest::getName() const
{
    return "sign-on exchange attributes";
}

}
