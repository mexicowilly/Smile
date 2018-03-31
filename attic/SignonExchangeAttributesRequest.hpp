#if !defined(SMILE_SIGNONEXCHANGEATTRIBUTESREQUEST_HPP__)
#define SMILE_SIGNONEXCHANGEATTRIBUTESREQUEST_HPP__

#include "PacketRequest.hpp"
#include <vector>

namespace smile
{

class SignonExchangeAttributesRequest : public PacketRequest
{
public:
    SignonExchangeAttributesRequest(const std::vector<uint8_t>& seed);

    virtual const char* getName() const;
};

}

#endif
