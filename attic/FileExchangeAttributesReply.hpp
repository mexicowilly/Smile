#if !defined(SMILE_FILEEXCHANGEATTRIBUTESREPLY_HPP__)
#define SMILE_FILEEXCHANGEATTRIBUTESREPLY_HPP__

#include "PacketReply.hpp"

namespace smile
{

class FileExchangeAttributesReply : public PacketReply
{
public:
    FileExchangeAttributesReply(const AccessPacket& packet);

    uint16_t getDataStreamLevel() const;
    size_t getMaxDataBlockSize() const;
    virtual const char* getName() const;
    int getPreferredCCSID() const;
};

inline FileExchangeAttributesReply::FileExchangeAttributesReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

inline uint16_t FileExchangeAttributesReply::getDataStreamLevel() const
{
    return m_packet.getInt16(22);
}

inline size_t FileExchangeAttributesReply::getMaxDataBlockSize() const
{
    return m_packet.getInt32(26);
}

inline int FileExchangeAttributesReply::getPreferredCCSID() const
{
    return m_packet.getInt16(36);
}

}

#endif
