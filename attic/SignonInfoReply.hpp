#if !defined(SMILE_SIGNONINFOREPLY_HPP__)
#define SMILE_SIGNONINFOREPLY_HPP__

#include "PacketReply.hpp"
#include "smile/Time.hpp"
#include <unicode/ucal.h>

namespace smile
{

class SignonInfoReply : public PacketReply
{
public:
    SignonInfoReply(const AccessPacket& packet);
    ~SignonInfoReply();

    int getCCSID() const;
    Time getCurrentSignonTime() const;
    Time getExpirationTime() const;
    Time getLastSignonTime() const;
    virtual const char* getName() const;
    std::vector<uint8_t> getUserId() const;

private:
    uint64_t getTime(uint16_t code) const;

    UCalendar* m_calendar;
};

inline Time SignonInfoReply::getCurrentSignonTime() const
{
    return getTime(0x1106);
}

inline Time SignonInfoReply::getExpirationTime() const
{
    return getTime(0x1108);
}

inline Time SignonInfoReply::getLastSignonTime() const
{
    return getTime(0x1107);
}

}

#endif
