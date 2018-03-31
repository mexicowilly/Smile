#if !defined(SMILE_SIGNONINFOREQUEST_HPP__)
#define SMILE_SIGNONINFOREQUEST_HPP__

#include "PacketRequest.hpp"
#include "AS400Impl.hpp"

namespace smile
{

class SignonInfoRequest : public PacketRequest
{
public:
    SignonInfoRequest(const std::vector<uint8_t>& userId,
                      const std::vector<uint8_t>& authentication,
                      AS400Impl::Authentication authenticationType,
                      uint16_t serverLevel);

    virtual const char* getName() const;
};

}

#endif
