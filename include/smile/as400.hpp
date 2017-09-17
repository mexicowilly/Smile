#ifndef SMILE_AS400_HPP
#define SMILE_AS400_HPP

#include <string>
#include <vector>

namespace smile
{

class as400
{
public:
    as400(const std::string& system_name,
          const std::string& user_id,
          const std::string& password);

private:
    enum class password_encryption
    {
        UNKNOWN,
        DES,
        SHA1
    };

    std::string system_name_;
    std::string user_id_;
    std::vector<std::uint8_t> auth_bytes_;
};

}

#endif
