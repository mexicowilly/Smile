#if !defined(SMILE_ENDSIGNONSERVICEREQUEST_HPP__)
#define SMILE_ENDSIGNONSERVICEREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class EndSignonServiceRequest : public PacketRequest
{
public:
    EndSignonServiceRequest();

    virtual const char* getName() const;
};

}

#endif
