#include <smile/as400.hpp>

#include "credential_store.hpp"
#include "enum_types.hpp"
#include "signon_service.hpp"

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
      auth_type_(+auth_type::PASSWORD)
{
    for (auto i = 0; i < io_threads; i++)
        threads_.emplace_back([this]() { io_.run(); });
}

as400::~as400()
{
    work_.reset();
    for (auto& t : threads_)
        t.join();
}

}