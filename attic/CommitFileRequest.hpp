#if !defined(SMILE_COMMITFILEREQUEST_HPP__)
#define SMILE_COMMITFILEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class CommitFileRequest : public PacketRequest
{
public:
    CommitFileRequest(uint32_t fileHandle);

    virtual const char* getName() const;
};

}

#endif
