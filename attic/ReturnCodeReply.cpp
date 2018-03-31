#include "ReturnCodeReply.hpp"

namespace smile
{

const char* ReturnCodeReply::getName() const
{
    return "return code";
}

uint32_t ReturnCodeReply::getReturnCode() const
{
    return m_packet.getInt16(22);
}

}
