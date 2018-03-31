#if !defined(SMILE_STARTSERVERREQUEST_HPP__)
#define SMILE_STARTSERVERREQUEST_HPP__

#include "PacketRequest.hpp"
#include "AS400Impl.hpp"
#include <vector>

namespace smile
{

class StartServerRequest : public PacketRequest
{
public:
    StartServerRequest(uint16_t serviceIdentifier,
                       const std::vector<uint8_t>& userIdBytes,
                       const std::vector<uint8_t>& authenticationBytes,
                       AS400Impl::Authentication authentication);

    virtual const char* getName() const;
};
}

#endif
