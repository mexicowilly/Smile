#include "DeleteDirectoryRequest.hpp"
#include "FileService.hpp"

namespace smile
{

DeleteDirectoryRequest::DeleteDirectoryRequest(const std::vector<uint8_t>& name, int ccsid)
    : PacketRequest(36 + name.size())
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(10);
    m_packet.setRequestReplyId(0x000e);
    m_packet.setInt16(22, ccsid);
    m_packet.setInt32(24, 1);
    m_packet.setInt32(30, name.size() + 6);
    m_packet.setInt16(34, 1);
    m_packet.setBytes(36, name);
}

const char* DeleteDirectoryRequest::getName() const
{
    return "delete directory";
}

}

