#if !defined(SMILE_INVALIDSTATEEXCEPTION_HPP__)
#define SMILE_INVALIDSTATEEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>

namespace smile
{

class InvalidStateException : public Exception
{
public:
    InvalidStateException(const char* const fileName,
                          int lineNumber,
                          const char* const msg);
    InvalidStateException(const InvalidStateException& except);

    InvalidStateException& operator= (const InvalidStateException& except);

    virtual Exception* duplicate() const;
};

}

#endif
