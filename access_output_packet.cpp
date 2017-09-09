#include "access_output_packet.hpp"
#include <assert.h>

namespace smile
{

access_output_packet::access_output_packet(std::uint16_t server_id,
                                           std::uint16_t template_length,
                                           std::uint16_t request_reply_id)
{
    skip(2);
    *this << server_id;
    skip(8);
    *this << template_length << request_reply_id;
    assert(bytes().size() == 20);
}

}