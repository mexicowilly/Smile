#ifndef SMILE_SERVICE_PORT_MAP_HPP
#define SMILE_SERVICE_PORT_MAP_HPP

#include <boost/asio.hpp>

#include <map>
#include <pthread.h>

namespace smile
{

class service_port_map
{
public:
    service_port_map(const std::string& host_name);

    std::uint16_t port(const char* const service_name);

private:
    boost::asio::io_service io_;
    std::map<const char*, std::uint16_t> map_;
    pthread_rwlock_t guard_;
    std::string host_name_;
};

}

#endif
