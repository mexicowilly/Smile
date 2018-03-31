#include "UnlockBytesRequest.hpp"
#include "FileService.hpp"

namespace
{

uint16_t getTemplateLength(uint16_t dataStreamLevel)
{
    return (dataStreamLevel < 16) ? 20 : 44;
}

}

namespace smile
{

UnlockBytesRequest::UnlockBytesRequest(uint32_t fileHandle,
                                       uint64_t offset,
                                       uint64_t length,
                                       uint16_t dataStreamLevel)
    : PacketRequest(20 + getTemplateLength(dataStreamLevel))
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(getTemplateLength(dataStreamLevel));
    m_packet.setRequestReplyId(0x0008);
    m_packet.setInt32(22, fileHandle);
    m_packet.setInt16(38, 1);
    if (dataStreamLevel < 16)
    {
        m_packet.setInt32(30, offset);
        m_packet.setInt32(34, length);
    }
    else
    {
        m_packet.setInt64(48, offset);
        m_packet.setInt64(56, length);
    }
}

const char* UnlockBytesRequest::getName() const
{
    return "unlock bytes";
}

}
