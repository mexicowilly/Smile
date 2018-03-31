#if !defined(SMILE_LOCKBYTESREPLY_HPP__)
#define SMILE_LOCKBYTESREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class LockBytesReply : public FileServiceReply
{
public:
    LockBytesReply(const AccessPacket& packet);

    virtual const char* getName() const;
    virtual uint32_t getReturnCode() const;
};

inline LockBytesReply::LockBytesReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

}

#endif
