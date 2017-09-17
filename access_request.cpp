#include "access_request.hpp"

namespace smile
{

access_request::access_request(std::uint16_t server_id,
                                      std::uint16_t template_length,
                                      std::uint16_t request_reply_id)
    : packet_(server_id, template_length, request_reply_id)
{
}

}