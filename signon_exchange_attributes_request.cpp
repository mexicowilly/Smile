#include "signon_exchange_attributes_request.hpp"
#include "signon_service.hpp"
#include "literal.hpp"

namespace smile
{

signon_exchange_attributes_request::signon_exchange_attributes_request(const std::vector<std::uint8_t>& seed)
    : access_request(signon_service::IDENTIFIER, 0, 0x7003)
{
    packet_ << 10_u32 << 0x1101_u16 << 1_u32 << 8_u32 << 0x1102_u16 << 2_u16;
    if (!seed.empty())
    {
        assert(seed.size() == 8);
        packet_ << 14_u32 << 0x1103_u16 << seed;
    }
}

}