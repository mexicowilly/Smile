#ifndef SMILE_ACCESS_REQUEST_HPP
#define SMILE_ACCESS_REQUEST_HPP

#include "request.hpp"
#include "access_output_packet.hpp"

namespace smile
{

class access_request : public request
{
public:
    access_request(std::uint16_t server_id,
                   std::uint16_t template_length,
                   std::uint16_t request_reply_id);

    const std::vector<std::uint8_t>& bytes() const;
    void finish();
    void set_correlation_id(std::uint32_t correlation_id);

protected:
    access_output_packet packet_;
};

inline const std::vector<std::uint8_t>& access_request::bytes() const
{
    return packet_.bytes();
}

inline void access_request::finish()
{
    packet_.finish();
}

inline void access_request::set_correlation_id(std::uint32_t correlation_id)
{
    packet_.set_correlation_id(correlation_id);
}

}

#endif
