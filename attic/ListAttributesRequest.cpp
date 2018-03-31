#include "ListAttributesRequest.hpp"
#include "FileService.hpp"

namespace smile
{

ListAttributesRequest::ListAttributesRequest(const std::vector<uint8_t>& fileName,
                                             int fileNameCCSID)
    : PacketRequest(46 + fileName.size())
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(20);
    m_packet.setRequestReplyId(0x000a);
    m_packet.setInt16(26, fileNameCCSID);
    m_packet.setInt32(28, 1);
    m_packet.setInt16(34, 0xffff);
    m_packet.setInt16(36, 0x0101);
    m_packet.setInt16(38, FileService::PatternMatching_PosixAll);
    m_packet.setInt32(40, fileName.size() + 6);
    m_packet.setInt16(44, 2);
    m_packet.setBytes(46, fileName);
}

const char* ListAttributesRequest::getName() const
{
    return "list attributes";
}

}
