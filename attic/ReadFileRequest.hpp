#if !defined(SMILE_READFILEREQUEST_HPP__)
#define SMILE_READFILEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class ReadFileRequest : public PacketRequest
{
public:
    ReadFileRequest(uint32_t fileHandle,
                    uint64_t offset,
                    uint32_t length,
                    uint16_t dataStreamLevel);

    virtual const char* getName() const;
};

}

#endif
