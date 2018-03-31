#include "SignonExchangeAttributesReply.hpp"

namespace smile
{

SignonExchangeAttributesReply::SignonExchangeAttributesReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

std::vector<uint8_t> SignonExchangeAttributesReply::getJobName() const
{
    int offset = findOffset(42, 0x111f);
    return (offset == -1) ?
        std::vector<uint8_t>() :
        m_packet.getBytes(offset + 10,
                          m_packet.getInt32(offset) - 10);
}

const char* SignonExchangeAttributesReply::getName() const
{
    return "sign-on exchange attributes";
}

AS400Impl::PasswordEncryption SignonExchangeAttributesReply::getPasswordEncryption() const
{
    int offset = findOffset(42, 0x1119);
    if (offset == -1)
        return AS400Impl::PasswordEncryption_DES;
    return (m_packet.getInt8(offset + 6) >= 2) ?
        AS400Impl::PasswordEncryption_SHA1 : AS400Impl::PasswordEncryption_DES;
}

std::vector<uint8_t> SignonExchangeAttributesReply::getServerSeed() const
{
    int offset = findOffset(42, 0x1103);
    if (offset == -1)
        return std::vector<uint8_t>();
    return (offset == -1) ?
        std::vector<uint8_t>() : m_packet.getBytes(offset + 6, 8);
}

}
