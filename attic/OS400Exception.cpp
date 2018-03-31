#include <smile/OS400Exception.hpp>
#include <sstream>

namespace smile
{

OS400Exception::OS400Exception(const char* const fileName,
                               int lineNumber,
                               const char* const msg,
                               const std::vector<OS400Message>& os400Messages)
    : Exception(fileName, lineNumber), m_os400Messages(os400Messages)
{
    std::ostringstream stream;
    stream << msg << ":" << std::endl;
    for (std::vector<OS400Message>::iterator itor = m_os400Messages.begin();
         itor != m_os400Messages.end();
         itor++)
    {
        stream << *itor << std::endl;
    }
    m_what = new char[stream.str().length() + 1];
    strcpy(m_what, stream.str().c_str());
}

OS400Exception::OS400Exception(const OS400Exception& except)
    : Exception(except), m_os400Messages(except.m_os400Messages)
{
}

OS400Exception::~OS400Exception() throw()
{
}

OS400Exception& OS400Exception::operator= (const OS400Exception& except)
{
    if (&except != this)
    {
        Exception::operator=(except);
        m_os400Messages = except.m_os400Messages;
    }
    return *this;
}

Exception* OS400Exception::duplicate() const
{
    return new OS400Exception(*this);
}

}
