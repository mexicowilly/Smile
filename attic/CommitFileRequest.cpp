#include "CommitFileRequest.hpp"
#include "FileService.hpp"

namespace smile
{

CommitFileRequest::CommitFileRequest(uint32_t fileHandle)
    : PacketRequest(26)
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(6);
    m_packet.setRequestReplyId(0x0006);
    m_packet.setInt32(22, fileHandle);
}


const char* CommitFileRequest::getName() const
{
    return "commit file";
}

}
