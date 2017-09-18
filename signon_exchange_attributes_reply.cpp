#include "signon_exchange_attributes_reply.hpp"

namespace smile
{

signon_exchange_attributes_reply::signon_exchange_attributes_reply(const access_input_packet& packet)
    : version_(packet.get<std::uint32_t>(30)),
      level_(packet.get<std::uint16_t>(40))
{
    auto found = packet.find(42, 0x1119);
    if (found == access_input_packet::npos)
        throw std::runtime_error("Could not find password type in reply");
    password_encryption_type_ = packet.get<std::uint8_t>(found + 6) >= 2 ?
        password_encryption_type::SHA1 : password_encryption_type::DES;
    found = packet.find(42, 0x1103);
    if (found == access_input_packet::npos)
        throw std::runtime_error("Could not find server seed in reply");
    seed_ = packet.get(found + 6, 8);
}

}