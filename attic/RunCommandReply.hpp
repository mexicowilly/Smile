#if !defined(SMILE_RUNCOMMANDREPLY_HPP__)
#define SMILE_RUNCOMMANDREPLY_HPP__

#include "MessageReply.hpp"

namespace smile
{

class RunCommandReply : public MessageReply
{
public:
    RunCommandReply(const AccessPacket& packet);

    virtual const char* getName() const;
};

inline RunCommandReply::RunCommandReply(const AccessPacket& packet)
    : MessageReply(packet)
{
}

}

#endif
