#if !defined(SMILE_WRITEFILEREQUEST_HPP__)
#define SMILE_WRITEFILEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class WriteFileRequest : public PacketRequest
{
public:
    WriteFileRequest(uint32_t fileHandle,
                     uint64_t offset,
                     const uint8_t* bytes,
                     uint32_t length,
                     int ccsid,
                     uint16_t dataStreamLevel,
                     bool isChain);

    virtual const char* getName() const;
};

}

#endif
