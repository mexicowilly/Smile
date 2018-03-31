#if !defined(SMILE_RETURNCODE16BITREPLY_HPP__)
#define SMILE_RETURNCODE16BITREPLY_HPP__

#include "PacketReply.hpp"

namespace smile
{

class ReturnCode16BitReply : public PacketReply
{
public:
    ReturnCode16BitReply(const AccessPacket& packet);

    virtual uint32_t getReturnCode() const;
};

inline ReturnCode16BitReply::ReturnCode16BitReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

}

#endif
