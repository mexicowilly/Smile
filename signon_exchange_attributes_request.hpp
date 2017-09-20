#ifndef SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REQUEST_HPP
#define SMILE_SIGNON_EXCHANGE_ATTRIBUTES_REQUEST_HPP

#include "access_request.hpp"

#include <boost/optional.hpp>

namespace smile
{

class signon_exchange_attributes_request : public access_request
{
public:
    signon_exchange_attributes_request(const boost::optional<std::array<std::uint8_t, 8>>& seed);
};

}

#endif
