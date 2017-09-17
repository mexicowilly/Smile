#include <smile/as400.hpp>

#include "credential_store.hpp"

namespace smile
{

as400::as400(const std::string& system_name,
             const std::string& user_id,
             const std::string& password)
    : system_name_(system_name),
      user_id_(user_id),
      cred_store_(new credential_store(password))
{
}

}