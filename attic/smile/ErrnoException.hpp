#if !defined(SMILE_ERRNOEXCEPTION_HPP__)
#define SMILE_ERRNOEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>

namespace smile
{

class ErrnoException : public Exception
{
public:
    ErrnoException(const char* const fileName,
                   int lineNumber,
                   const char* const msg,
                   int err);
    ErrnoException(const ErrnoException& except);

    ErrnoException& operator= (const ErrnoException& except);

    virtual Exception* duplicate() const;
    int getErrno() const;

private:
    int m_errno;
};

inline int ErrnoException::getErrno() const
{
    return m_errno;
}

}

#endif
