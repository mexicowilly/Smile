#include "StartServerRequest.hpp"

namespace smile
{

StartServerRequest::StartServerRequest(uint16_t serviceIdentifier,
                                       const std::vector<uint8_t>& userIdBytes,
                                       const std::vector<uint8_t>& authenticationBytes,
                                       AS400Impl::Authentication authentication)
    : PacketRequest((userIdBytes.empty() ? 28 : 44) + authenticationBytes.size())
{
    m_packet.setServerId(serviceIdentifier);
    m_packet.setTemplateLength(2);
    m_packet.setRequestReplyId(0x7002);
    m_packet.setInt8(4, 2);
//    data_[20] = (byteType == AS400.AUTHENTICATION_SCHEME_PASSWORD) ? (authenticationBytes.length == 8) ? (byte)0x01 : (byte)0x03 : (byteType == AS400.AUTHENTICATION_SCHEME_GSS_TOKEN) ? (byte)0x05 : (byteType == AS400.AUTHENTICATION_SCHEME_IDENTITY_TOKEN) ? (byte)0x06 : (byte)0x02;
    assert(authentication == AS400Impl::Authentication_Password);
    m_packet.setInt8(20, (authenticationBytes.size() == 8) ? 1 : 3);
    m_packet.setInt8(21, 1);
    m_packet.setInt32(22, authenticationBytes.size() + 6);
    m_packet.setInt16(26, 0x1105);
    // offset 26 to 0x1115 if authentication type is not password
    m_packet.setBytes(28, authenticationBytes);
    if (userIdBytes.size() != 0)
    {
        m_packet.setInt32(28 + authenticationBytes.size(), 16);
        m_packet.setInt16(32 + authenticationBytes.size(), 0x1104);
        assert(userIdBytes.size() == 10);
        m_packet.setBytes(34 + authenticationBytes.size(), userIdBytes);
    }
}

const char* StartServerRequest::getName() const
{
    return "start server";
}

}
