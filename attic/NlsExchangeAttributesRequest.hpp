#if !defined(SMILE_NLSEXCHANGEATTRIBUTESREQUEST_HPP__)
#define SMILE_NLSEXCHANGEATTRIBUTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class NlsExchangeAttributesRequest : public PacketRequest
{
public:
    NlsExchangeAttributesRequest();

    virtual const char* getName() const;
};

}

#endif
