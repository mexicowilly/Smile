#include "signon_info_request.hpp"
#include "signon_service.hpp"
#include "literal.hpp"

namespace smile
{

signon_info_request::signon_info_request(const std::vector<std::uint8_t>& user_id,
                                         const std::vector<std::uint8_t>& authentication,
                                         auth_type auth_t,
                                         std::uint16_t server_level)
    : access_request(signon_service::IDENTIFIER, 1, 0x7004)
{
    packet_ << ((authentication.size() == 8) ? 1_u8 : 3_u8);
    packet_ << 10_u32;
    packet_ << 0x1113_u16;
    // We will always use UTF-16BE when talking to the server
    packet_ << 1200_u32;
    std::uint32_t as = authentication.size() + 6;
    packet_ << as;
    if(auth_t == auth_type::PASSWORD)
        packet_ << 0x1105_u16;
    else
        packet_ << 0x1115_u16;
    packet_ << authentication;
    if (!user_id.empty())
    {
        packet_ << 16_u32;
        packet_ << 0x1104_u16;
        assert(user_id.size() == 10);
        packet_ << user_id;
    }
    if (server_level >= 5)
    {
        packet_ << 7_u32;
        packet_ << 0x1128_u16;
        packet_ << 1_u8;
    }
}

}