#include "SignonInfoReply.hpp"
#include <smile/Exception.hpp>

namespace smile
{

SignonInfoReply::SignonInfoReply(const AccessPacket& packet)
    : PacketReply(packet)
{
    UErrorCode uerr = U_ZERO_ERROR;
    m_calendar = ucal_open(0, 0, uloc_getDefault(), UCAL_GREGORIAN, &uerr);
    if (U_FAILURE(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not open calendar", uerr);
}

SignonInfoReply::~SignonInfoReply()
{
    ucal_close(m_calendar);
}

int SignonInfoReply::getCCSID() const
{
    int offset = findOffset(24, 0x1114);
    return (offset < 0) ? 0 : m_packet.getInt32(offset + 6);
}

Time SignonInfoReply::getTime(uint16_t code) const
{
    int offset = findOffset(24, code);
    if (offset < 0)
        return 0;
    int32_t year = m_packet.getInt16(offset + 6);
    int32_t month = m_packet.getInt8(offset + 8) - 1;
    int32_t day = m_packet.getInt8(offset + 9);
    int32_t hour = m_packet.getInt8(offset + 10);
    int32_t minute = m_packet.getInt8(offset + 11);
    int32_t second = m_packet.getInt8(offset + 12);
    UErrorCode uerr = U_ZERO_ERROR;
    ucal_setDateTime(m_calendar, year, month, day, hour, minute, second, &uerr);
    if (U_FAILURE(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not set date", uerr);
    uerr = U_ZERO_ERROR;
    UDate millis = ucal_getMillis(m_calendar, &uerr);
    if (U_FAILURE(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not get time in milliseconds", uerr);
    return static_cast<Time>(millis);
}

const char* SignonInfoReply::getName() const
{
    return "sign-on info";
}

std::vector<uint8_t> SignonInfoReply::getUserId() const
{
    int offset = findOffset(24, 0x1104);
    if (offset < 0)
        return std::vector<uint8_t>();
    std::vector<uint8_t> output = m_packet.getBytes(offset + 10, m_packet.getInt32(offset) - 10);
    output.resize(10, 0x40);
    return output;
}

}
