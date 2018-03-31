#if !defined(SMILE_ENDJOBREQUEST_HPP__)
#define SMILE_ENDJOBREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class EndJobRequest : public PacketRequest
{
public:
    EndJobRequest(uint16_t serverId);

    virtual const char* getName() const;
};

}

#endif
