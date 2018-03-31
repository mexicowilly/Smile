#if !defined(SMILE_SIGNONEXCHANGEATTRIBUTESREPLY_HPP__)
#define SMILE_SIGNONEXCHANGEATTRIBUTESREPLY_HPP__

#include "PacketReply.hpp"
#include "smile/OS400Version.hpp"
#include "AS400Impl.hpp"

namespace smile
{

class SignonExchangeAttributesReply : public PacketReply
{
public:
    SignonExchangeAttributesReply(const AccessPacket& packet);

    std::vector<uint8_t> getJobName() const;
    virtual const char* getName() const;
    OS400Version getOS400Version() const;
    uint16_t getOS400Level() const;
    AS400Impl::PasswordEncryption getPasswordEncryption() const;
    std::vector<uint8_t> getServerSeed() const;
};

inline OS400Version SignonExchangeAttributesReply::getOS400Version() const
{
    return OS400Version(m_packet.getInt32(30));
}

inline uint16_t SignonExchangeAttributesReply::getOS400Level() const
{
    return m_packet.getInt16(40);
}

}

#endif
