#include "CloseFileRequest.hpp"
#include "FileService.hpp"

namespace smile
{

CloseFileRequest::CloseFileRequest(uint32_t fileHandle)
    : PacketRequest(41)
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(21);
    m_packet.setRequestReplyId(0x0009);
    m_packet.setInt32(22, fileHandle);
    m_packet.setInt16(26, 2);
    m_packet.setInt16(28, 0xffff);
    m_packet.setInt16(30, 100);
}

const char* CloseFileRequest::getName() const
{
    return "close file";
}

}
