#include "access_input_packet.hpp"

namespace smile
{

access_input_packet::access_input_packet(const std::vector<std::uint8_t>& bytes)
    : input_packet(&bytes[0], bytes.size())
{
    skip(20);
}

}