#include <smile/ErrnoException.hpp>
#include <string.h>

namespace smile
{

ErrnoException::ErrnoException(const char* const fileName,
                               int lineNumber,
                               const char* const msg,
                               int err)
    : Exception(fileName, lineNumber), m_errno(err)
{
    const char* errMsg = strerror(m_errno);
    m_what = new char[strlen(msg) + 2 + strlen(errMsg) + 1];
    strcpy(m_what, msg);
    strcat(m_what, ": ");
    strcat(m_what, errMsg);
}

ErrnoException::ErrnoException(const ErrnoException& except)
    : Exception(except), m_errno(except.m_errno)
{
}

ErrnoException& ErrnoException::operator= (const ErrnoException& except)
{
    Exception::operator=(except);
    m_errno = except.m_errno;
    return *this;
}

Exception* ErrnoException::duplicate() const
{
    return new ErrnoException(*this);
}

}
