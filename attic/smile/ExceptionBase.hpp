#if !defined(SMILE_EXCEPTIONBASE_HPP__)
#define SMILE_EXCEPTIONBASE_HPP__

#include <smile/Duplicable.hpp>
#include <exception>
#include <ostream>

namespace smile
{

class Exception : public std::exception, public Duplicable<Exception>
{
public:
    Exception(const char* const fileName,
              int lineNumber,
              const char* const msg);
    Exception(const Exception& except);
    virtual ~Exception() throw();

    Exception& operator= (const Exception& except);

    virtual Exception* duplicate() const;
    const char* getFileName() const;
    int getLineNumber() const;
    const char* what() const throw();

protected:
    Exception(const char* const fileName, int lineNumber);

    char* m_fileName;
    char* m_what;
    int m_lineNumber;
};

inline const char* Exception::getFileName() const
{
    return m_fileName;
}

inline int Exception::getLineNumber() const
{
    return m_lineNumber;
}

}

std::ostream& operator<< (std::ostream& stream, const smile::Exception& except);

#endif
