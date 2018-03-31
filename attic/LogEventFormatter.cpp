#include <smile/LogEventFormatter.hpp>
#include <map>

namespace smile
{

const char* LogEventFormatter::getLevelName(LogLevel level) const
{
    static std::map<LogLevel, const char*> levelNames;
    if (levelNames.empty())
    {
        levelNames[LogLevel_All] = "";
        levelNames[LogLevel_Trace] = "TRACE";
        levelNames[LogLevel_Debug] = "DEBUG";
        levelNames[LogLevel_Info] = "INFO";
        levelNames[LogLevel_Warn] = "WARN";
        levelNames[LogLevel_Error] = "ERROR";
        levelNames[LogLevel_Fatal] = "FATAL";
        levelNames[LogLevel_None] = "";
    }
    return levelNames[level];
}

size_t LogEventFormatter::getMaxLevelNameLength() const
{
    return 5;
}

}
