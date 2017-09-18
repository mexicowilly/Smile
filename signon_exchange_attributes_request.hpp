#ifndef SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REQUEST_HPP
#define SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REQUEST_HPP

#include "access_request.hpp"

namespace smile
{

class signon_exchange_attributes_request : public access_request
{
    signon_exchange_attributes_request(const std::vector<std::uint8_t>& seed);
};

}

#endif
