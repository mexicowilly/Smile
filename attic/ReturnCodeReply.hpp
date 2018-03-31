#if !defined(SMILE_RETURNCODEREPLY_HPP__)
#define SMILE_RETURNCODEREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class ReturnCodeReply : public FileServiceReply
{
public:
    ReturnCodeReply(const AccessPacket& packet);

    virtual const char* getName() const;
    virtual uint32_t getReturnCode() const;
};

inline ReturnCodeReply::ReturnCodeReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

}

#endif
