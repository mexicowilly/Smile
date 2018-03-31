#if !defined(SMILE_LISTATTRIBUTESREQUEST_HPP__)
#define SMILE_LISTATTRIBUTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class ListAttributesRequest : public PacketRequest
{
public:
    ListAttributesRequest(const std::vector<uint8_t>& fileName,
                          int fileNameCCSID);

    virtual const char* getName() const;
};

}

#endif
