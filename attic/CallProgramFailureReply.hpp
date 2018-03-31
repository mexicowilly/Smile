#if !defined(SMILE_CALLPROGRAMFAILUREREPLY_HPP__)
#define SMILE_CALLPROGRAMFAILUREREPLY_HPP__

#include "CallProgramReply.hpp"

namespace smile
{

class CallProgramFailureReply : public CallProgramReply
{
public:
    CallProgramFailureReply(const AccessPacket& packet);

    virtual const char* getName() const;
};

inline CallProgramFailureReply::CallProgramFailureReply(const AccessPacket& packet)
    : CallProgramReply(packet)
{
}

}

#endif
