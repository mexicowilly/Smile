#if !defined(SMILE_REMOTECOMMANDEXCHANGEATTRIBUTESREQUEST_HPP__)
#define SMILE_REMOTECOMMANDEXCHANGEATTRIBUTESREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class RemoteCommandExchangeAttributesRequest : public PacketRequest
{
public:
    RemoteCommandExchangeAttributesRequest();

    virtual const char* getName() const;
};

}

#endif
