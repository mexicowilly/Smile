#if !defined(SMILE_OS400EXCEPTION_HPP__)
#define SMILE_OS400EXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>
#include <smile/OS400Message.hpp>

namespace smile
{

class OS400Exception : public Exception
{
public:
    OS400Exception(const char* const fileName,
                   int lineNumber,
                   const char* const msg,
                   const std::vector<OS400Message>& os400Messages);
    OS400Exception(const OS400Exception& except);
    virtual ~OS400Exception() throw();

    OS400Exception& operator= (const OS400Exception& except);

    virtual Exception* duplicate() const;
    const std::vector<OS400Message>& getOS400Messages() const;

private:
    std::vector<OS400Message> m_os400Messages;
};

inline const std::vector<OS400Message>& OS400Exception::getOS400Messages() const
{
    return m_os400Messages;
}

}

#endif
