#include "LockBytesReply.hpp"

namespace smile
{

const char* LockBytesReply::getName() const
{
    return "lock bytes";
}

uint32_t LockBytesReply::getReturnCode() const
{
    return m_packet.getInt16(22);
}

}
