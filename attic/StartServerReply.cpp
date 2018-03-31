#include "StartServerReply.hpp"

namespace smile
{

StartServerReply::StartServerReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

std::vector<uint8_t> StartServerReply::getJobName() const
{
    int offset = findOffset(24, 0x111f);
    if (offset == -1)
        return std::vector<uint8_t>();
    return m_packet.getBytes(offset + 10, m_packet.getInt32(offset) - 10);
}

const char* StartServerReply::getName() const
{
    return "start server";
}

std::vector<uint8_t> StartServerReply::getUserId() const
{
    int offset = findOffset(24, 0x1104);
    if (offset == -1)
        return std::vector<uint8_t>();
    std::vector<uint8_t> output = m_packet.getBytes(offset + 10, m_packet.getInt32(offset) - 10);
    output.insert(output.end(), output.size() - 10, 0x40);
    return output;
}

}
