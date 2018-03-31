#include "ReturnCode16BitReply.hpp"

namespace smile
{

uint32_t ReturnCode16BitReply::getReturnCode() const
{
    return m_packet.getInt16(20);
}

}
