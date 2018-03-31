#if !defined(SMILE_ACCESSPACKET_HPP__)
#define SMILE_ACCESSPACKET_HPP__

#include "Packet.hpp"

namespace smile
{

class AccessPacket : public Packet
{
public:
    AccessPacket(size_t size);
    AccessPacket(const AccessPacket& packet);

    AccessPacket& operator= (const AccessPacket& packet);

    uint32_t getCorrelation() const;
    uint32_t getCsInstance() const;
    uint16_t getHeaderId() const;
    uint16_t getRequestReplyId() const;
    uint16_t getServerId() const;
    uint16_t getTemplateLength() const;
    virtual void read(Connection& connection);
    void setCorrelation(uint32_t value);
    void setCsInstance(uint32_t value);
    void setHeaderId(uint16_t value);
    void setRequestReplyId(uint16_t value);
    void setServerId(uint16_t value);
    void setTemplateLength(uint16_t value);
};

inline AccessPacket::AccessPacket(size_t size)
    : Packet(size)
{
}

inline AccessPacket::AccessPacket(const AccessPacket& packet)
    : Packet(packet)
{
}

inline AccessPacket& AccessPacket::operator= (const AccessPacket& packet)
{
    Packet::operator=(packet);
    return *this;
}

inline uint32_t AccessPacket::getCorrelation() const
{
    return getInt32(12);
}

inline uint32_t AccessPacket::getCsInstance() const
{
    return getInt32(8);
}

inline uint16_t AccessPacket::getHeaderId() const
{
    return getInt16(4);
}

inline uint16_t AccessPacket::getRequestReplyId() const
{
    return getInt16(18);
}

inline uint16_t AccessPacket::getServerId() const
{
    return getInt16(6);
}

inline uint16_t AccessPacket::getTemplateLength() const
{
    return getInt16(16);
}

inline void AccessPacket::setCorrelation(uint32_t value)
{
    setInt32(12, value);
}

inline void AccessPacket::setCsInstance(uint32_t value)
{
    setInt32(8, value);
}

inline void AccessPacket::setHeaderId(uint16_t value)
{
    setInt16(4, value);
}

inline void AccessPacket::setRequestReplyId(uint16_t value)
{
    setInt16(18, value);
}

inline void AccessPacket::setServerId(uint16_t value)
{
    setInt16(6, value);
}

inline void AccessPacket::setTemplateLength(uint16_t value)
{
    setInt16(16, value);
}

}

#endif
