#ifndef SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REPLY_HPP
#define SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REPLY_HPP

#include <smile/os400_version.hpp>

#include "access_reply.hpp"
#include "access_input_packet.hpp"
#include "enum_types.hpp"

namespace smile
{

class signon_exchange_attributes_reply : public access_reply
{
public:
    signon_exchange_attributes_reply(const access_input_packet& packet);

    password_encryption_type password_encryption() const;
    std::uint16_t server_level() const;
    const std::vector<std::uint8_t>& server_seed() const;
    const os400_version& version() const;

private:
    password_encryption_type password_encryption_type_;
    os400_version version_;
    std::uint16_t level_;
    std::vector<std::uint8_t> seed_;
};

inline password_encryption_type signon_exchange_attributes_reply::password_encryption() const
{
    return password_encryption_type_;
}

inline std::uint16_t signon_exchange_attributes_reply::server_level() const
{
    return level_;
}

inline const std::vector<std::uint8_t>& signon_exchange_attributes_reply::server_seed() const
{
    return seed_;
}

inline const os400_version& signon_exchange_attributes_reply::version() const
{
    return version_;
}

}

#endif
