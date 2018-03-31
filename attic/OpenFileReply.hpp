#if !defined(SMILE_OPENFILEREPLY_HPP__)
#define SMILE_OPENFILEREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class OpenFileReply : public FileServiceReply
{
public:
    OpenFileReply(const AccessPacket& packet);

    Time getAccessTime() const;
    Time getCreateTime() const;
    uint32_t getFileHandle() const;
    uint64_t getFileSize(uint16_t dataStreamLevel) const;
    Time getModificationTime() const;
    virtual const char* getName() const;
};

inline OpenFileReply::OpenFileReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

inline Time OpenFileReply::getAccessTime() const
{
    return getTime(54);
}

inline Time OpenFileReply::getCreateTime() const
{
    return getTime(38);
}

inline uint32_t OpenFileReply::getFileHandle() const
{
    return m_packet.getInt32(22);
}

inline uint64_t OpenFileReply::getFileSize(uint16_t dataStreamLevel) const
{
    return (dataStreamLevel < 16) ?
        m_packet.getInt32(62) : m_packet.getInt64(89);
}

inline Time OpenFileReply::getModificationTime() const
{
    return getTime(46);
}

}

#endif
