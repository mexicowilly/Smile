#if !defined(SMILE_ICUEXCEPTION_HPP__)
#define SMILE_ICUEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>
#include <unicode/utypes.h>

namespace smile
{

class IcuException : public Exception
{
public:
    IcuException(const char* const fileName,
                 int lineNumber,
                 const char* const msg,
                 UErrorCode uErrorCode);
    IcuException(const IcuException& except);

    IcuException& operator= (const IcuException& except);

    virtual Exception* duplicate() const;
    UErrorCode getUErrorCode() const;

private:
    UErrorCode m_uErrorCode;
};

inline UErrorCode IcuException::getUErrorCode() const
{
    return m_uErrorCode;
}

}

#endif
