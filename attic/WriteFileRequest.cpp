#include "WriteFileRequest.hpp"
#include "FileService.hpp"

namespace
{

uint16_t getTemplateLength(uint16_t dataStreamLevel)
{
    return dataStreamLevel < 16 ? 18 : 34;
}

}

namespace smile
{

WriteFileRequest::WriteFileRequest(uint32_t fileHandle,
                                   uint64_t offset,
                                   const uint8_t* bytes,
                                   uint32_t length,
                                   int ccsid,
                                   uint16_t dataStreamLevel,
                                   bool isChain)
    : PacketRequest(20 + getTemplateLength(dataStreamLevel) + 6 + length)
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(getTemplateLength(dataStreamLevel));
    m_packet.setRequestReplyId(0x0004);
    m_packet.setInt16(20, isChain ? 1 : 0);
    m_packet.setInt32(22, fileHandle);
    if (dataStreamLevel < 16)
    {
        m_packet.setInt32(30, offset);
        m_packet.setInt32(38, length + 6);
        m_packet.setInt16(42, 0x0020);
        m_packet.setBytes(44, bytes, length);
    }
    else
    {
        m_packet.setInt64(46, offset);
        m_packet.setInt32(54, length + 6);
        m_packet.setInt16(58, 0x0020);
        m_packet.setBytes(60, bytes, length);
    }
    m_packet.setInt16(34, 2);
    m_packet.setInt16(36, ccsid);
}

const char* WriteFileRequest::getName() const
{
    return "write file";
}

}
