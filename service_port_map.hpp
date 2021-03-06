#ifndef SMILE_SERVICE_PORT_MAP_HPP
#define SMILE_SERVICE_PORT_MAP_HPP

#include <boost/asio.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <map>

namespace smile
{

class service_port_map
{
public:
    service_port_map(const std::string& host_name);

    std::uint16_t port(const char* const service_name);

private:
    boost::asio::io_service io_;
    std::map<std::string, std::uint16_t> map_;
    boost::shared_mutex guard_;
    std::string host_name_;
};

}

#endif
