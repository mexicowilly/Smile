#if !defined(SMILE_CLOSEFILEREPLY_HPP__)
#define SMILE_CLOSEFILEREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class CloseFileReply : public FileServiceReply
{
public:
    CloseFileReply(const AccessPacket& packet);

    virtual const char* getName() const;
    virtual uint32_t getReturnCode() const;
};

inline CloseFileReply::CloseFileReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

}

#endif
