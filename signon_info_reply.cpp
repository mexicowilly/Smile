#include "signon_info_reply.hpp"
#include "icu_exception.hpp"
#include "signon_text.hpp"

#include <unicode/ucal.h>
#include <boost/scope_exit.hpp>

namespace smile
{

smile::signon_info_reply::clock::time_point get_time(const access_input_packet& packet,
                                                     std::uint16_t code,
                                                     UCalendar* cal)
{
    auto off = packet.find(24, code);
    if (off == smile::access_input_packet::npos)
        throw std::runtime_error("Could not find time position in signon_info_reply");
    int32_t year = packet.get<std::uint16_t>(off + 6);
    int32_t month = packet.get<std::uint8_t>(off + 8) - 1;
    int32_t day = packet.get<std::uint8_t>(off + 9);
    int32_t hour = packet.get<std::uint8_t>(off + 10);
    int32_t minute = packet.get<std::uint8_t>(off + 11);
    int32_t second = packet.get<std::uint8_t>(off + 12);
    UErrorCode uerr = U_ZERO_ERROR;
    ucal_setDateTime(cal, year, month, day, hour, minute, second, &uerr);
    if (U_FAILURE(uerr))
        throw smile::icu_exception("Could not set date", uerr);
    uerr = U_ZERO_ERROR;
    UDate millis = ucal_getMillis(cal, &uerr);
    if (U_FAILURE(uerr))
        throw smile::icu_exception("Could not get time in milliseconds", uerr);
    return smile::signon_info_reply::clock::time_point(smile::signon_info_reply::clock::from_time_t(millis / 1000));
}

}

namespace smile
{

signon_info_reply::signon_info_reply(const access_input_packet& packet)
{
    UErrorCode uerr = U_ZERO_ERROR;
    UCalendar* cal = ucal_open(0, 0, uloc_getDefault(), UCAL_GREGORIAN, &uerr);
    if (U_FAILURE(uerr))
        throw icu_exception("Could not open calendar", uerr);
    BOOST_SCOPE_EXIT(&cal)
    {
        ucal_close(cal);
    } BOOST_SCOPE_EXIT_END
    auto off = packet.find(24, 0x1114);
    ccsid_ = (off == access_input_packet::npos) ? 0 : packet.get<std::uint32_t>(off + 6);
    current_signon_time_ = get_time(packet, 0x1106, cal);
    expiration_time_ = get_time(packet, 0x1108, cal);
    last_signon_time_ = get_time(packet, 0x1107, cal);
    off = packet.find(24, 0x1104);
    if (off == access_input_packet::npos)
        throw std::runtime_error("Could not find user_id position in signon_info_reply");
    auto bytes = packet.get(off + 10, packet.get<std::uint32_t>(off) - 10);
    bytes.resize(10, 0x40);
    user_id_ = signon_text::from_signon_ebcdic_to_utf8(bytes);
}

}