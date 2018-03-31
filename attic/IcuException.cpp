#include <smile/IcuException.hpp>

namespace smile
{

IcuException::IcuException(const char* const fileName,
                           int lineNumber,
                           const char* const msg,
                           UErrorCode uErrorCode)
    : Exception(fileName, lineNumber), m_uErrorCode(uErrorCode)
{
    const char* errName = u_errorName(m_uErrorCode);
    m_what = new char[strlen(msg) + 2 + strlen(errName) + 1];
    strcpy(m_what, msg);
    strcat(m_what, ": ");
    strcat(m_what, errName);
}

IcuException::IcuException(const IcuException& except)
    : Exception(except), m_uErrorCode(except.m_uErrorCode)
{
}

IcuException& IcuException::operator= (const IcuException& except)
{
    Exception::operator=(except);
    m_uErrorCode = except.m_uErrorCode;
    return *this;
}

Exception* IcuException::duplicate() const
{
    return new IcuException(*this);
}

}
