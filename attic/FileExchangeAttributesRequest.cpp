#include "FileExchangeAttributesRequest.hpp"
#include "FileService.hpp"

namespace
{

const size_t HEADER_LENGTH = 20;
const size_t TEMPLATE_LENGTH = 10;
const size_t LL_CP_LENGTH = 6;
const size_t CCSID_LENGTH = 2;

}

namespace smile
{

FileExchangeAttributesRequest::FileExchangeAttributesRequest(const std::vector<int>& preferredCCSIDs,
                                                             uint16_t dataStreamLevel)
    : PacketRequest(HEADER_LENGTH + TEMPLATE_LENGTH +
                    LL_CP_LENGTH + (CCSID_LENGTH * preferredCCSIDs.size()))
{
    m_packet.setServerId(FileService::IDENTIFIER);
    m_packet.setTemplateLength(TEMPLATE_LENGTH);
    m_packet.setRequestReplyId(0x0016);
    m_packet.setInt16(22, dataStreamLevel);
    // Use Posix return codes, GMT times and Posix all pattern matching
    m_packet.setInt16(24, 8 | 4 | FileService::PatternMatching_PosixAll);
    // Max data block size is real big
    m_packet.setInt32(26, UINT32_MAX);
    m_packet.setInt32(30, LL_CP_LENGTH + (CCSID_LENGTH * preferredCCSIDs.size()));
    m_packet.setInt16(34, 0x000a);
    unsigned offset = 36;
    for (std::vector<int>::const_iterator itor = preferredCCSIDs.begin();
         itor != preferredCCSIDs.end();
         itor++)
    {
        m_packet.setInt16(offset, *itor);
        offset += CCSID_LENGTH;
    }
}

const char* FileExchangeAttributesRequest::getName() const
{
    return "file exchange attributes";
}

}
