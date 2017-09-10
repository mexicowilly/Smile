#ifndef SMILE_ACCESS_OUTPUT_PACKET_HPP
#define SMILE_ACCESS_OUTPUT_PACKET_HPP

#include "output_packet.hpp"

namespace smile
{

class access_output_packet : public output_packet
{
public:
    access_output_packet(std::uint16_t server_id,
                         std::uint16_t template_length,
                         std::uint16_t request_reply_id);

    void set_correlation_id(std::uint32_t id);
};

inline void access_output_packet::set_correlation_id(std::uint32_t id)
{
    set(12, id);
}

}

#endif
