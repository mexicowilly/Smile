#if !defined(SMILE_LOCKBYTESREQUEST_HPP__)
#define SMILE_LOCKBYTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class LockBytesRequest : public PacketRequest
{
public:
    LockBytesRequest(uint32_t fileHandle,
                     uint64_t offset,
                     uint64_t length,
                     uint16_t dataStreamLevel);

    virtual const char* getName() const;
};

}

#endif
