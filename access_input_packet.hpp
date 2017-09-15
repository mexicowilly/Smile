#ifndef SMILE_ACCESS_INPUT_PACKET_HPP
#define SMILE_ACCESS_INPUT_PACKET_HPP

#include "input_packet.hpp"

namespace smile
{

class access_input_packet : public input_packet
{
public:
    access_input_packet(const std::vector<std::uint8_t>& bytes);

    std::uint32_t correlation_id() const;
    std::uint16_t request_reply_id() const;
    std::uint16_t template_length() const;
};

inline std::uint32_t access_input_packet::correlation_id() const
{
    return get<std::uint32_t>(12);
}

inline std::uint16_t access_input_packet::request_reply_id() const
{
    return get<std::uint16_t>(18);
}

inline std::uint16_t access_input_packet::template_length() const
{
    return get<std::uint16_t>(16);
}

}

#endif
