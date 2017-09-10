#include "service_port_map.hpp"

#include <boost/endian/conversion.hpp>

#include <stdexcept>

namespace smile
{

service_port_map::service_port_map(const std::string& host_name)
    : guard_(PTHREAD_RWLOCK_INITIALIZER),
      host_name_(host_name)
{
}

std::uint16_t service_port_map::port(const char* const service_name)
{
    std::uint16_t result;
    pthread_rwlock_rdlock(&guard_);
    auto found = map_.find(service_name);
    if (found != map_.end())
    {
        result = found->second;
        pthread_rwlock_unlock(&guard_);
    }
    else
    {
        pthread_rwlock_unlock(&guard_);
        boost::asio::ip::tcp::resolver resolver(io_);
        boost::asio::ip::tcp::resolver::query query(host_name_, std::to_string(449));
        auto itor = resolver.resolve(query);
        boost::asio::ip::tcp::socket sock(io_);
        boost::asio::connect(sock, itor);
        boost::asio::write(sock, boost::asio::buffer(service_name, std::strlen(service_name)));
        std::uint8_t id;
        boost::asio::read(sock, boost::asio::buffer(&id, 1));
        if (id != 0x2b)
            throw std::runtime_error("Unexpected response from port request");
        std::uint32_t p;
        boost::asio::read(sock, boost::asio::buffer(&p, sizeof(p)));
        boost::endian::big_to_native_inplace(p);
        pthread_rwlock_wrlock(&guard_);
        map_[service_name] = p;
        pthread_rwlock_unlock(&guard_);
        result = p;
    }
    return result;
}

}