#include <smile/LogEvent.hpp>
#include <smile/Exception.hpp>
#include <sys/time.h>
#include <errno.h>

namespace smile
{

LogEvent::LogEvent(const std::string& loggerName, const std::string& message, LogLevel level)
    : m_loggerName(loggerName), m_message(message), m_level(level)
{
    struct timeval now;
    if (gettimeofday(&now, 0) != 0)
        throw ErrnoException(__FILE__, __LINE__, "Could not get current time", errno);
    m_time = (static_cast<Time>(now.tv_sec) * 1000UL) +
             (static_cast<Time>(now.tv_usec) / 1000UL);
}

LogEvent::LogEvent(const LogEvent& event)
    : m_loggerName(event.m_loggerName),
      m_message(event.m_message),
      m_level(event.m_level),
      m_time(event.m_time)
{
}

LogEvent& LogEvent::operator= (const LogEvent& event)
{
    if (&event != this)
    {
        m_loggerName = event.m_loggerName;
        m_message = event.m_message;
        m_level = event.m_level;
        m_time = event.m_time;
    }
    return *this;
}

}
