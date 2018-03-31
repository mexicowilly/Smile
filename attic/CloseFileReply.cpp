#include "CloseFileReply.hpp"

namespace smile
{

const char* CloseFileReply::getName() const
{
    return "close file";
}

uint32_t CloseFileReply::getReturnCode() const
{
    return m_packet.getInt16(22);
}

}
