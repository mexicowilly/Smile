#ifndef SMILE_SIGNON_INFO_REPLY_HPP
#define SMILE_SIGNON_INFO_REPLY_HPP

#include "access_reply.hpp"
#include "access_input_packet.hpp"

#include <memory>
#include <chrono>

namespace smile
{

class signon_info_reply : public access_reply
{
public:
    using clock = std::chrono::system_clock;

    signon_info_reply(const access_input_packet& packet);

    std::uint16_t ccsid() const;
    const clock::time_point& current_signon_time() const;
    const clock::time_point& expiration_time() const;
    const clock::time_point& last_signon_time() const;
    const std::string& user_id() const;

private:
    std::uint16_t ccsid_;
    clock::time_point current_signon_time_;
    clock::time_point expiration_time_;
    clock::time_point last_signon_time_;
    std::string user_id_;
};

inline std::uint16_t signon_info_reply::ccsid() const
{
    return ccsid_;
}

inline const signon_info_reply::clock::time_point& signon_info_reply::current_signon_time() const
{
    return current_signon_time_;
}

inline const signon_info_reply::clock::time_point& signon_info_reply::expiration_time() const
{
    return expiration_time_;
}

inline const signon_info_reply::clock::time_point& signon_info_reply::last_signon_time() const
{
    return last_signon_time_;
}

inline const std::string& signon_info_reply::user_id() const
{
    return user_id_;
}

}

#endif
