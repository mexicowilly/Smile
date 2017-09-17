#ifndef SMILE_SIGNON_SERVICE_HPP
#define SMILE_SIGNON_SERVICE_HPP

#include "service.hpp"

namespace smile
{

class signon_service : public service
{
public:
    static constexpr const std::uint16_t IDENTIFIER = 0xe009;

    signon_service(boost::asio::io_service& io,
                   const std::string& system_name,
                   service_port_map& port_map);
    virtual ~signon_service();

    virtual std::uint16_t id() const override;
    virtual const char* name() const override;

protected:
    virtual std::unique_ptr<access_reply> packet_to_reply(const access_input_packet& packet) const override;
};

}

#endif
