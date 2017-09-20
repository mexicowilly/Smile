#include <smile/as400.hpp>

#include "credential_store.hpp"
#include "enum_types.hpp"
#include "signon_service.hpp"
#include "signon_exchange_attributes_request.hpp"
#include "signon_exchange_attributes_reply.hpp"

#include <openssl/rand.h>

namespace smile
{

as400::as400(std::size_t io_threads,
             const std::string& system_name,
             const std::string& user_id,
             const std::string& password)
    : work_(new boost::asio::io_service::work(io_)),
      system_name_(system_name),
      user_id_(user_id),
      cred_store_(new credential_store(password)),
      password_encryption_type_(+password_encryption_type::UNKNOWN),
      auth_type_(+auth_type::PASSWORD),
      service_port_map_(new service_port_map(system_name))
{
    for (auto i = 0; i < io_threads; i++)
        threads_.emplace_back([this]() { io_.run(); });
    RAND_bytes(client_seed_.data(), client_seed_.size());
    while (client_seed_[0] >= 0xe8)
        RAND_bytes(client_seed_.data(), 1);
    signon_service svc(io_, system_name, *service_port_map_);
    svc.connect(std::chrono::milliseconds(1000));
    boost::optional<std::array<std::uint8_t, 8>> seed;
    if (auth_type_ == +auth_type::PASSWORD)
        seed = client_seed_;
    signon_exchange_attributes_request req(seed);
    auto correlation = svc.send(req);
    auto rep = cast_reply<signon_exchange_attributes_reply>(svc.receive(correlation, std::chrono::milliseconds(1000)));
    version_ = rep->version();
    server_level_ = rep->server_level();
    password_encryption_type_ = +rep->password_encryption();
    std::copy(rep->server_seed().begin(), rep->server_seed().end(), server_seed_.begin());
}

as400::~as400()
{
    work_.reset();
    for (auto& t : threads_)
        t.join();
}

}