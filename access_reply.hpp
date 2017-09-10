#ifndef SMILE_ACCESS_REPLY_HPP
#define SMILE_ACCESS_REPLY_HPP

#include "reply.hpp"
#include "access_input_packet.hpp"

namespace smile
{

class access_reply : public reply, public access_input_packet
{
public:
    access_reply(const std::uint8_t* const bytes, std::size_t s);
};

inline access_reply::access_reply(const std::uint8_t* const bytes, std::size_t s)
    : access_input_packet(bytes, s)
{
}

}

#endif
