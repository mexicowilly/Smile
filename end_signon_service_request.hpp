#ifndef SMILE_END_SIGNON_SERVER_REQUEST_HPP
#define SMILE_END_SIGNON_SERVER_REQUEST_HPP

#include "access_request.hpp"
#include "signon_service.hpp"

namespace smile
{

class end_signon_service_request : public access_request
{
public:
    end_signon_service_request();
};

inline end_signon_service_request::end_signon_service_request()
    : access_request(signon_service::IDENTIFIER, 0, 0x7006)
{
}

}

#endif
