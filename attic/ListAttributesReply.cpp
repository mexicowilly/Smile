#include "ListAttributesReply.hpp"

namespace
{

enum ObjectType
{
    ObjectType_File             = 1,
    ObjectType_Directory        = 2,
    ObjectType_SymbolicLink     = 3,
    ObjectType_Object           = 4,
    ObjectType_FifoDevice       = 5,
    ObjectType_CharDevice       = 6,
    ObjectType_BlockDevice      = 7,
    ObjectType_Socket           = 8
};

static const uint32_t FIXED_DIRECTORY = 0x10;

}

namespace smile
{

const char* ListAttributesReply::getName() const
{
    return "list attributes";
}

std::vector<uint8_t> ListAttributesReply::getObjectName() const
{
    unsigned offset = 20 + m_packet.getTemplateLength();
    return m_packet.getBytes(offset + 6, m_packet.getInt32(offset) - 6);
}

uint64_t ListAttributesReply::getSize(uint16_t dataStreamLevel) const
{
    uint64_t fileSize = 0;
    if (dataStreamLevel < 8 || dataStreamLevel == 0xf4f4)
    {
        unsigned cur = 20 + m_packet.getTemplateLength();
        uint32_t curLength = m_packet.getInt32(cur);
        uint16_t curCode = m_packet.getInt16(cur + 4);
        while (curCode != 0x0014 &&
               (cur + curLength + 6) <= m_packet.getSize())
        {
            cur += curLength;
            curLength = m_packet.getInt32(cur);
            curCode = m_packet.getInt16(cur + 4);
        }
        if (curCode == 0x0014)
            fileSize = m_packet.getInt64(cur + 6);
        else
            fileSize = m_packet.getInt32(46);
    }
    else
    {
        fileSize = m_packet.getInt64(81);
    }
    return fileSize;
}

bool ListAttributesReply::isDirectory() const
{
    ObjectType type = static_cast<ObjectType>(m_packet.getInt16(54));
    return (type == ObjectType_Directory || type == ObjectType_File) &&
           ((m_packet.getInt32(50) & FIXED_DIRECTORY) != 0);
}

}
