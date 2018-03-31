#include "LockBytesRequest.hpp"
#include "FileService.hpp"

namespace smile
{

LockBytesRequest::LockBytesRequest(uint32_t fileHandle,
                                   uint64_t offset,
                                   uint64_t length,
                                   uint16_t dataStreamLevel)
    : PacketRequest(28 + (dataStreamLevel < 16) ? 20 : 32)
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(8);
    m_packet.setRequestReplyId(0x0007);
    m_packet.setInt32(22, fileHandle);
    m_packet.setInt16(26, 1);
    m_packet.setInt16(32, 6);
    if (dataStreamLevel < 16)
    {
        m_packet.setInt32(28, 20);
        m_packet.setInt32(40, offset);
        m_packet.setInt32(44, length);
    }
    else
    {
        m_packet.setInt32(28, 32);
        m_packet.setInt64(44, offset);
        m_packet.setInt64(52, length);
    }
}

const char* LockBytesRequest::getName() const
{
    return "lock bytes";
}

}
