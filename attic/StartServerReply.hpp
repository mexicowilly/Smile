#if !defined(SMILE_STARTSERVERREPLY_HPP__)
#define SMILE_STARTSERVERREPLY_HPP__

#include "PacketReply.hpp"

namespace smile
{

class StartServerReply : public PacketReply
{
public:
    StartServerReply(const AccessPacket& packet);

    std::vector<uint8_t> getJobName() const;
    virtual const char* getName() const;
    std::vector<uint8_t> getUserId() const;
};

}

#endif
