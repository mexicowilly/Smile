#if !defined(SMILE_CLOSEFILEREQUEST_HPP__)
#define SMILE_CLOSEFILEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class CloseFileRequest : public PacketRequest
{
public:
    CloseFileRequest(uint32_t fileHandle);

    virtual const char* getName() const;
};

}
#endif
