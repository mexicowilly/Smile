#if !defined(SMILE_WRITEFILEREPLY_HPP__)
#define SMILE_WRITEFILEREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class WriteFileReply : public FileServiceReply
{
public:
    WriteFileReply(const AccessPacket& packet);

    uint32_t getLengthNotWritten() const;
    virtual const char* getName() const;
    virtual uint32_t getReturnCode() const;
};

inline WriteFileReply::WriteFileReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

inline uint32_t WriteFileReply::getLengthNotWritten() const
{
    return m_packet.getInt32(28);
}

}

#endif
