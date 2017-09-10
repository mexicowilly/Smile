#ifndef SMILE_ACCESS_REQUEST_HPP
#define SMILE_ACCESS_REQUEST_HPP

#include "request.hpp"
#include "access_output_packet.hpp"

namespace smile
{

class access_request : public request, public access_output_packet
{
public:
    access_request(std::uint16_t server_id,
                   std::uint16_t template_length,
                   std::uint16_t request_reply_id);
};

inline access_request::access_request(std::uint16_t server_id,
                                      std::uint16_t template_length,
                                      std::uint16_t request_reply_id)
    : access_output_packet(server_id, template_length, request_reply_id)
{
}

}

#endif
