#ifndef SMILE_SIGNON_INFO_REQUEST_HPP
#define SMILE_SIGNON_INFO_REQUEST_HPP

#include "access_request.hpp"
#include "auth_type.hpp"

namespace smile
{

class signon_info_request : public access_request
{
public:
    signon_info_request(const std::vector<std::uint8_t>& user_id,
                        const std::vector<std::uint8_t>& authentication,
                        auth_type auth_t,
                        std::uint16_t server_level);
};

}

#endif
