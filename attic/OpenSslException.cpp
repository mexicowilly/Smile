#include <smile/OpenSslException.hpp>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <pthread.h>

namespace
{

pthread_once_t openSslOnce = PTHREAD_ONCE_INIT;

void openSslInit()
{
    ERR_load_BIO_strings();
    SSL_load_error_strings();
}

}

namespace smile
{

OpenSslException::OpenSslException(const char* const fileName,
                                   int lineNumber,
                                   const char* const msg)
    : Exception(fileName, lineNumber), m_openSslError(ERR_get_error())
{
    char buf[256];
    pthread_once(&openSslOnce, openSslInit);
    ERR_error_string(m_openSslError, buf);
    m_what = new char[strlen(msg) + 2 + strlen(buf) + 1];
    strcpy(m_what, msg);
    strcat(m_what, ": ");
    strcat(m_what, buf);
}

OpenSslException::OpenSslException(const OpenSslException& except)
    : Exception(except), m_openSslError(except.m_openSslError)
{
}

OpenSslException& OpenSslException::operator= (const OpenSslException& except)
{
    if (&except != this)
    {
        Exception::operator=(except);
        m_openSslError = except.m_openSslError;
    }
    return *this;
}

Exception* OpenSslException::duplicate() const
{
    return new OpenSslException(*this);
}

}
