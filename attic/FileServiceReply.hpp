#if !defined(SMILE_FILEREPLY_HPP__)
#define SMILE_FILEREPLY_HPP__

#include "PacketReply.hpp"
#include "smile/Time.hpp"

namespace smile
{

class FileServiceReply : public PacketReply
{
public:
    bool isEndOfChain() const;

protected:
    FileServiceReply(const AccessPacket& packet);

    Time getTime(unsigned offset) const;
};

inline FileServiceReply::FileServiceReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

inline bool FileServiceReply::isEndOfChain() const
{
    return (m_packet.getInt16(20) & 1) == 0;
}

}

#endif
