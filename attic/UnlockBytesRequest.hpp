#if !defined(SMILE_UNLOCKBYTESREQUEST_HPP__)
#define SMILE_UNLOCKBYTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class UnlockBytesRequest : public PacketRequest
{
public:
    UnlockBytesRequest(uint32_t fileHandle,
                       uint64_t offset,
                       uint64_t length,
                       uint16_t dataStreamLevel);

    virtual const char* getName() const;
};

}

#endif
