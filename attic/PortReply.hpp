#if !defined(SMILE_PORTREPLY_HPP__)
#define SMILE_PORTREPLY_HPP__

#include "Reply.hpp"
#include <inttypes.h>

namespace smile
{

class PortReply : public Reply
{
public:
    virtual const char* getName() const;
    int getPort() const;
    virtual void receive(Connection& connection);

private:
    uint8_t m_bytes[5];
};

}

#endif
