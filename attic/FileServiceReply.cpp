#include "FileServiceReply.hpp"

namespace smile
{

Time FileServiceReply::getTime(unsigned offset) const
{
    return (m_packet.getInt32(offset) * 1000) +
        (m_packet.getInt32(offset + 4) / 1000);
}

}
