#if !defined(SMILE_SIGNONENDSERVERREQUEST_HPP__)
#define SMILE_SIGNONENDSERVERREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class SignonEndServerRequest : public PacketRequest
{
public:
    SignonEndServerRequest();

    virtual const char* getName() const;
};

}

#endif
