#if !defined(SMILE_READFILEREPLY_HPP__)
#define SMILE_READFILEREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class ReadFileReply : public FileServiceReply
{
public:
    ReadFileReply(const AccessPacket& packet);

    std::vector<uint8_t> getData() const;
    virtual const char* getName() const;
};

inline std::vector<uint8_t> ReadFileReply::getData() const
{
    return m_packet.getBytes(30, m_packet.getInt32(24) - 6);
}

inline ReadFileReply::ReadFileReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

}

#endif
