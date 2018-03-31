#if !defined(SMILE_DELETEFILEREQUEST_HPP__)
#define SMILE_DELETEFILEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class DeleteFileRequest : public PacketRequest
{
public:
    DeleteFileRequest(const std::vector<uint8_t>& name, int ccsid);

    virtual const char* getName() const;
};

}

#endif

