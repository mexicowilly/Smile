#include "DeleteFileRequest.hpp"
#include "FileService.hpp"

namespace smile
{

DeleteFileRequest::DeleteFileRequest(const std::vector<uint8_t>& name, int ccsid)
    : PacketRequest(34 + name.size())
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(8);
    m_packet.setRequestReplyId(0x000c);
    m_packet.setInt16(22, ccsid);
    m_packet.setInt32(24, 1);
    m_packet.setInt32(28, name.size() + 6);
    m_packet.setInt16(32, 2);
    m_packet.setBytes(34, name);
}

const char* DeleteFileRequest::getName() const
{
    return "delete file";
}

}
