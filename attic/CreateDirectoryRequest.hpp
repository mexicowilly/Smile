#if !defined(SMILE_CREATEDIRECTORYREQUEST_HPP__)
#define SMILE_CREATEDIRECTORYREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class CreateDirectoryRequest : public PacketRequest
{
public:
    CreateDirectoryRequest(const std::vector<uint8_t>& name, int ccsid);

    virtual const char* getName() const;
};

}

#endif
