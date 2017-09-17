#ifndef SMILE_AS400_HPP
#define SMILE_AS400_HPP

#include <string>
#include <vector>
#include <memory>

namespace smile
{

class credential_store;

class as400
{
public:
    as400(const std::string& system_name,
          const std::string& user_id,
          const std::string& password);
    as400(const as400&) = delete;

    as400& operator= (const as400&) = delete;

private:
    enum class password_encryption
    {
        UNKNOWN,
        DES,
        SHA1
    };

    std::string system_name_;
    std::string user_id_;
    std::unique_ptr<credential_store> cred_store_;
};

}

#endif
