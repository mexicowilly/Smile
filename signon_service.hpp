#ifndef SMILE_SIGNON_SERVICE_HPP
#define SMILE_SIGNON_SERVICE_HPP

#include "service.hpp"

namespace smile
{

class signon_service : public service
{
public:
    static constexpr const std::uint16_t IDENTIFIER = 0xe009;

};

}

#endif
