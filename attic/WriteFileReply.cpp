#include "WriteFileReply.hpp"

namespace smile
{

const char* WriteFileReply::getName() const
{
    return "write file";
}

uint32_t WriteFileReply::getReturnCode() const
{
    return m_packet.getInt16(22);
}

}
