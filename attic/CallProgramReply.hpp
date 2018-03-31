#if !defined(SMILE_CALLPROGRAMREPLY_HPP__)
#define SMILE_CALLPROGRAMREPLY_HPP__

#include "MessageReply.hpp"
#include "smile/ProgramParameterList.hpp"

namespace smile
{

class CallProgramReply : public MessageReply
{
public:
    CallProgramReply(const AccessPacket& packet);

    virtual const char* getName() const;
    void imbueOutputParameters(ProgramParameterList& parameters);
};

inline CallProgramReply::CallProgramReply(const AccessPacket& packet)
    : MessageReply(packet)
{
}

}

#endif
