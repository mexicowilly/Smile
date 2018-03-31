#if !defined(SMILE_PACKETREPLY_HPP__)
#define SMILE_PACKETREPLY_HPP__

#include "Reply.hpp"
#include "AccessPacket.hpp"

namespace smile
{

class PacketReply : public Reply
{
public:
    PacketReply(const AccessPacket& packet);
    PacketReply(const PacketReply& reply);

    PacketReply& operator= (const PacketReply& reply);

    uint32_t getCorrelation() const;
    virtual const char* getName() const;
    virtual uint32_t getReturnCode() const;
    virtual void receive(Connection& connection);

protected:
    int findOffset(unsigned start, uint16_t code) const;

    AccessPacket m_packet;
};

inline uint32_t PacketReply::getCorrelation() const
{
    return m_packet.getCorrelation();
}

}

#endif
