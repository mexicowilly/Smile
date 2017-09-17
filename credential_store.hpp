#ifndef SMILE_CREDENTIAL_STORE_HPP
#define SMILE_CREDENTIAL_STORE_HPP

#include <string>
#include <vector>

namespace smile
{

class credential_store
{
public:
    credential_store(const std::string& pass);

    std::string get();

private:
    std::vector<std::uint8_t> encoded_;
};

}

#endif
