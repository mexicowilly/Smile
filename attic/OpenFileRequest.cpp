#include "OpenFileRequest.hpp"
#include "FileService.hpp"

namespace
{

uint16_t getTemplateLength(uint16_t dataStreamLevel)
{
    return (dataStreamLevel < 16) ? 36 : 44;
}

}

namespace smile
{

OpenFileRequest::OpenFileRequest(const std::vector<uint8_t>& fileName,
                                 int ccsid,
                                 IfsFileHandle::Access accessMode,
                                 IfsFileHandle::OpenMode openMode,
                                 uint16_t dataStreamLevel)
    : PacketRequest(26 + getTemplateLength(dataStreamLevel) + fileName.size())
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(getTemplateLength(dataStreamLevel));
    m_packet.setRequestReplyId(0x0002);
    m_packet.setInt16(22, ccsid);
    m_packet.setInt32(24, 1);
    m_packet.setInt16(30, accessMode);
    m_packet.setInt16(36, openMode);
    m_packet.setInt16(46, 1);
    m_packet.setInt32((dataStreamLevel < 16) ? 56 : 64, fileName.size() + 6);
    m_packet.setInt16((dataStreamLevel < 16) ? 60 : 68, 0x0002);
    m_packet.setBytes((dataStreamLevel < 16) ? 62 : 70, fileName);
}

const char* OpenFileRequest::getName() const
{
    return "open file";
}

}
