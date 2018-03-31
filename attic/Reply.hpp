#if !defined(SMILE_REPLY_HPP__)
#define SMILE_REPLY_HPP__

#include "Connection.hpp"

namespace smile
{

class Reply
{
public:
    virtual ~Reply();

    virtual const char* getName() const = 0;
    virtual void receive(Connection& connection) = 0;
};

}

#endif
