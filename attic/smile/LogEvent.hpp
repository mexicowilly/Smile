#if !defined(SMILE_LOGEVENT_HPP__)
#define SMILE_LOGEVENT_HPP__

#include <smile/LogLevel.hpp>
#include <smile/Time.hpp>
#include <string>

namespace smile
{

class LogEvent
{
public:
    LogEvent(const std::string& loggerName, const std::string& message, LogLevel level);
    LogEvent(const LogEvent& event);

    LogEvent& operator= (const LogEvent& event);

    const std::string& getLoggerName() const;
    LogLevel getLogLevel() const;
    const std::string& getMessage() const;
    Time getTime() const;

private:
    std::string m_loggerName;
    std::string m_message;
    LogLevel m_level;
    Time m_time;
};

inline const std::string& LogEvent::getLoggerName() const
{
    return m_loggerName;
}

inline LogLevel LogEvent::getLogLevel() const
{
    return m_level;
}

inline const std::string& LogEvent::getMessage() const
{
    return m_message;
}

inline Time LogEvent::getTime() const
{
    return m_time;
}

}

#endif
