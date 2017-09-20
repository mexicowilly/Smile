#ifndef SMILE_AS400_HPP
#define SMILE_AS400_HPP

#include "os400_version.hpp"

#include <boost/asio.hpp>

#include <string>
#include <vector>
#include <memory>
#include <thread>

namespace smile
{

class credential_store;
class service_port_map;

class as400
{
public:
    using clock = std::chrono::system_clock;

    as400(std::size_t io_threads,
          const std::string& system_name,
          const std::string& user_id,
          const std::string& password);
    as400(const as400&) = delete;
    virtual ~as400();

    as400& operator= (const as400&) = delete;

private:
    boost::asio::io_service io_;
    std::unique_ptr<boost::asio::io_service::work> work_;
    std::vector<std::thread> threads_;
    std::string system_name_;
    std::string user_id_;
    std::unique_ptr<credential_store> cred_store_;
    int password_encryption_type_;
    int auth_type_;
    std::uint16_t ccsid_;
    os400_version version_;
    std::uint16_t server_level_;
    std::unique_ptr<service_port_map> service_port_map_;
    std::array<std::uint8_t, 8> client_seed_;
    std::array<std::uint8_t, 8> server_seed_;
};

}

#endif
