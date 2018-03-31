#if !defined(SMILE_EXCHANGERANDOMSEEDSREPLY_HPP__)
#define SMILE_EXCHANGERANDOMSEEDSREPLY_HPP__

#include "PacketReply.hpp"

namespace smile
{

class ExchangeRandomSeedsReply : public PacketReply
{
public:
    ExchangeRandomSeedsReply(const AccessPacket& packet);

    virtual const char* getName() const;
    std::vector<uint8_t> getServerSeed() const;
};

inline ExchangeRandomSeedsReply::ExchangeRandomSeedsReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

inline std::vector<uint8_t> ExchangeRandomSeedsReply::getServerSeed() const
{
    return m_packet.getBytes(24, 8);
}

}

#endif
