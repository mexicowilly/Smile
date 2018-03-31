#include <smile/SimpleLogEventFormatter.hpp>
#include <sstream>
#include <iomanip>
#include <sys/time.h>

namespace smile
{

SimpleLogEventFormatter::SimpleLogEventFormatter()
{
}

LogEventFormatter* SimpleLogEventFormatter::duplicate() const
{
    return new SimpleLogEventFormatter();
}

std::string SimpleLogEventFormatter::format(const LogEvent& event)
{
    struct timeval now;
    gettimeofday(&now, 0);
    struct tm nowParts;
    localtime_r(&now.tv_sec, &nowParts);
    unsigned millis = now.tv_usec / 1000;
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(4) << nowParts.tm_year + 1900 << '-' <<
        std::setw(2) << nowParts.tm_mon + 1 << '-' <<
        std::setw(2) << nowParts.tm_mday << ' ' <<
        std::setw(2) << nowParts.tm_hour << ':' <<
        std::setw(2) << nowParts.tm_min << ':' <<
        std::setw(2) << nowParts.tm_sec << ':' <<
        std::setw(3) << millis << ' ' <<
        std::setfill(' ') << std::setw(getMaxLevelNameLength()) << std::left <<
        getLevelName(event.getLogLevel()) <<
        ' ' << event.getLoggerName() << " - " << event.getMessage();
    return stream.str();
}

}
