#if !defined(SMILE_PACKETREQUEST_HPP__)
#define SMILE_PACKETREQUEST_HPP__

#include "Request.hpp"
#include "AccessPacket.hpp"

namespace smile
{

class PacketRequest : public Request
{
public:
    PacketRequest(size_t size);
    PacketRequest(const PacketRequest& request);

    PacketRequest& operator= (const PacketRequest& request);

    uint32_t getCorrelation() const;
    virtual void send(Connection& connection);
    void setCorrelation(uint32_t correlation);

protected:
    AccessPacket m_packet;
};

inline uint32_t PacketRequest::getCorrelation() const
{
    return m_packet.getCorrelation();
}

inline void PacketRequest::setCorrelation(uint32_t correlation)
{
    m_packet.setCorrelation(correlation);
}

}

#endif
