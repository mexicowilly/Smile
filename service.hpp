#ifndef SMILE_SERVICE_HPP
#define SMILE_SERVICE_HPP

#include "service_port_map.hpp"

#include <boost/asio.hpp>
#include <chucho/loggable.hpp>

#include <string>

namespace smile
{

/**
 * Service will be created as a weak pointer by as400. Users of
 * service will have to periodically check that their weak pointer
 * is still valid.
 *
 * All I/O done through boost::asio must be async from this class
 * and its subclasses.
 */
class service : public std::enable_shared_from_this<service>, public chucho::loggable<service>
{
public:
    service(boost::asio::io_service& io,
            const std::string& system_name,
            service_port_map& port_map);

    void connect();

    virtual const char* name() const = 0;
    virtual std::uint16_t id() const = 0;

protected:
    boost::asio::io_service& io_;

private:
    void connect_handler(std::shared_ptr<service> myself,
                         const boost::system::error_code& ec);
    void resolve_handler(std::shared_ptr<service> myself,
                         const boost::system::error_code& ec,
                         boost::asio::ip::tcp::resolver::iterator itor);

    service_port_map& port_map_;
    std::string system_name_;
    boost::asio::ip::tcp::socket sock_;
};

}

#endif
