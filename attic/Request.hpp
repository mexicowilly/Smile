#if !defined(SMILE_REQUEST_HPP__)
#define SMILE_REQUEST_HPP__

#include "Connection.hpp"

namespace smile
{

class Request
{
public:
    virtual ~Request();

    virtual const char* getName() const = 0;
    virtual void send(Connection& connection) = 0;
};

}

#endif
