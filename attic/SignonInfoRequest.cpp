#include "SignonInfoRequest.hpp"
#include "SignonService.hpp"

namespace smile
{

SignonInfoRequest::SignonInfoRequest(const std::vector<uint8_t>& userId,
                                     const std::vector<uint8_t>& authentication,
                                     AS400Impl::Authentication authenticationType,
                                     uint16_t serverLevel)
    : PacketRequest(37 + authentication.size() + (userId.empty() ? 0 : 16) + ((serverLevel < 5) ? 0 : 7))
{
    m_packet.setServerId(SignonService::IDENTIFIER);
    m_packet.setTemplateLength(1);
    m_packet.setRequestReplyId(0x7004);
    m_packet.setInt8(20, (authentication.size() == 8) ? 1 : 3);
    m_packet.setInt32(21, 10);
    m_packet.setInt16(25, 0x1113);
    // We will always use UTF-16BE when talking to the server
    m_packet.setInt32(27, 1200);
    m_packet.setInt32(31, 6 + authentication.size());
    if (authenticationType == AS400Impl::Authentication_Password)
        m_packet.setInt16(35, 0x1105);
    else
        m_packet.setInt16(35, 0x1115);
    m_packet.setBytes(37, authentication);
    if (!userId.empty())
    {
        m_packet.setInt32(37 + authentication.size(), 16);
        m_packet.setInt16(41 + authentication.size(), 0x1104);
        assert(userId.size() == 10);
        m_packet.setBytes(43 + authentication.size(), userId);
    }
    if (serverLevel >= 5)
    {
        unsigned offset = 37 + authentication.size() + (userId.empty() ? 0 : 16);
        m_packet.setInt32(offset, 7);
        m_packet.setInt16(offset + 4, 0x1128);
        m_packet.setInt8(offset + 6, 1);
    }
}

const char* SignonInfoRequest::getName() const
{
    return "sign-on info";
}

}
