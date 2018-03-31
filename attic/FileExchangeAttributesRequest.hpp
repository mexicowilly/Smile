#if !defined(SMILE_FILEEXCHANGEATTRIBUTESREQUEST_HPP__)
#define SMILE_FILEEXCHANGEATTRIBUTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class FileExchangeAttributesRequest : public PacketRequest
{
public:
    FileExchangeAttributesRequest(const std::vector<int>& preferredCCSIDs,
                                  uint16_t dataStreamLevel);

    virtual const char* getName() const;
};

}

#endif
