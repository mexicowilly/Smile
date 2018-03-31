#include <hominid/AssertionFailure.hpp>
#include <sstream>

namespace hominid
{

AssertionFailure::AssertionFailure(const char* const file, const int line)
    : m_file(file), m_message(), m_line(line)
{
}

AssertionFailure::AssertionFailure(const char* const file, const int line, const std::string& message)
    : m_file(file), m_message(message), m_line(line)
{
}

AssertionFailure::AssertionFailure(const AssertionFailure& failure)
    : m_file(failure.m_file), m_message(failure.m_message), m_line(failure.m_line)
{
}

AssertionFailure& AssertionFailure::operator= (const AssertionFailure& failure)
{
    if (this != &failure)
    {
        m_file = failure.m_file;
        m_message = failure.m_message;
        m_line = failure.m_line;
    }
    return *this;
}

std::string AssertionFailure::toString() const
{
    std::ostringstream strm;
    strm << "(" << m_file << "," << m_line << ") " << m_message;
    return strm.str();
}

}
