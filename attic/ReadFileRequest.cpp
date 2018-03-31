#include "ReadFileRequest.hpp"
#include "FileService.hpp"

namespace
{

uint16_t getTemplateLength(uint16_t dataStreamLevel)
{
    return dataStreamLevel < 16 ? 22 : 38;
}

}

namespace smile
{

ReadFileRequest::ReadFileRequest(uint32_t fileHandle,
                                 uint64_t offset,
                                 uint32_t length,
                                 uint16_t dataStreamLevel)
    : PacketRequest(20 + getTemplateLength(dataStreamLevel))
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(getTemplateLength(dataStreamLevel));
    m_packet.setRequestReplyId(0x0003);
    m_packet.setInt32(22, fileHandle);
    if (dataStreamLevel < 16)
        m_packet.setInt32(30, offset);
    else
        m_packet.setInt64(50, offset);
    m_packet.setInt32(34, length);
}

const char* ReadFileRequest::getName() const
{
    return "read file";
}

}
