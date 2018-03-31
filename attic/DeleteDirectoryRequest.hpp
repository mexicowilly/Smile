#if !defined(SMILE_DELETEDIRECTORYREQUEST_HPP__)
#define SMILE_DELETEDIRECTORYREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class DeleteDirectoryRequest : public PacketRequest
{
public:
    DeleteDirectoryRequest(const std::vector<uint8_t>& name, int ccsid);

    virtual const char* getName() const;
};

}

#endif


