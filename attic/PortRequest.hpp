#if !defined(SMILE_PORTREQUEST_HPP__)
#define SMILE_PORTREQUEST_HPP__

#include "Request.hpp"
#include <inttypes.h>
#include <vector>

namespace smile
{

class PortRequest : public Request
{
public:
    PortRequest(const char* const serviceName);

    virtual const char* getName() const;
    virtual void send(Connection& connection);

private:
    std::vector<uint8_t> m_ascii;
};

}

#endif
