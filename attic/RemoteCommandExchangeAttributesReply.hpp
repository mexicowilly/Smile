#if !defined(SMILE_REMOTECOMMANDEXCHANGEATTRIBUTESREPLY_HPP__)
#define SMILE_REMOTECOMMANDEXCHANGEATTRIBUTESREPLY_HPP__

#include "ReturnCode16BitReply.hpp"

namespace smile
{

class RemoteCommandExchangeAttributesReply : public ReturnCode16BitReply
{
public:
    RemoteCommandExchangeAttributesReply(const AccessPacket& packet);

    int getCCSID() const;
    uint16_t getDataStreamLevel() const;
    virtual const char* getName() const; 
};

inline RemoteCommandExchangeAttributesReply::RemoteCommandExchangeAttributesReply(const AccessPacket& packet)
    : ReturnCode16BitReply(packet)
{
}

inline int RemoteCommandExchangeAttributesReply::getCCSID() const
{
    return m_packet.getInt32(22);
}

inline uint16_t RemoteCommandExchangeAttributesReply::getDataStreamLevel() const
{
    return m_packet.getInt16(34);
}

}

#endif
