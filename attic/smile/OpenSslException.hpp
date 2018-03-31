#if !defined(SMILE_OPENSSLEXCEPTION_HPP__)
#define SMILE_OPENSSLEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>

namespace smile
{

class OpenSslException : public Exception
{
public:
    OpenSslException(const char* const fileName,
                     int lineNumber,
                     const char* const msg);
    OpenSslException(const OpenSslException& except);

    OpenSslException& operator= (const OpenSslException& except);

    virtual Exception* duplicate() const;

private:
    unsigned long m_openSslError;
};

}

#endif
