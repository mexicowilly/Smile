#ifndef SMILE_SERVICE_HPP
#define SMILE_SERVICE_HPP

#include <cstdint>

namespace smile
{

class service
{
public:
    virtual const char* name() const = 0;
    virtual uint16_t id() const = 0;
};

}

#endif
