#include <smile/InvalidStateException.hpp>

namespace smile
{

InvalidStateException::InvalidStateException(const char* const fileName,
                                                    int lineNumber,
                                                    const char* const msg)
    : Exception(fileName, lineNumber, msg)
{
}

InvalidStateException::InvalidStateException(const InvalidStateException& except)
    : Exception(except)
{
}

InvalidStateException& InvalidStateException::operator= (const InvalidStateException& except)
{
    Exception::operator=(except);
    return *this;
}

Exception* InvalidStateException::duplicate() const
{
    return new InvalidStateException(*this);
}

}
