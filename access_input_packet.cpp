#include "access_input_packet.hpp"

namespace smile
{

access_input_packet::access_input_packet(const std::uint8_t* const bytes, std::size_t s)
    : input_packet(bytes, s)
{
    skip(20);
}

}