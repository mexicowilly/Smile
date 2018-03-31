#if !defined(SMILE_EXCHANGERANDOMSEEDSREQUEST_HPP__)
#define SMILE_EXCHANGERANDOMSEEDSREQUEST_HPP__

#include "PacketRequest.hpp"

namespace smile
{

class ExchangeRandomSeedsRequest : public PacketRequest
{
public:
    ExchangeRandomSeedsRequest(uint16_t serverId);

    std::vector<uint8_t> getClientSeed() const;
    virtual const char* getName() const;
};

inline std::vector<uint8_t> ExchangeRandomSeedsRequest::getClientSeed() const
{
    return m_packet.getBytes(20, 8);
}

}

#endif
