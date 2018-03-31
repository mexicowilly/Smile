#if !defined(SMILE_NLSEXCHANGEATTRIBUTESREPLY_HPP__)
#define SMILE_NLSEXCHANGEATTRIBUTESREPLY_HPP__

#include "PacketReply.hpp"

namespace smile
{

class NlsExchangeAttributesReply : public PacketReply
{
public:
    NlsExchangeAttributesReply(const AccessPacket& packet);

    int getCCSID() const;
    virtual const char* getName() const;
};

inline NlsExchangeAttributesReply::NlsExchangeAttributesReply(const AccessPacket& packet)
    : PacketReply(packet)
{
}

inline int NlsExchangeAttributesReply::getCCSID() const
{
    return m_packet.getInt32(28);
}

}

#endif
