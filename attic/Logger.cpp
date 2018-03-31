#include <smile/Logger.hpp>
#include "Locker.hpp"
#include "DeletePointer.hpp"
#include "StaticInitializer.hpp"
#include <stdarg.h>
#include <vector>
#include <algorithm>

namespace
{

class WriteEvent : public std::unary_function<smile::LogWriter*, void>
{
public:
    WriteEvent(const smile::LogEvent& event);

    void operator() (smile::LogWriter*& writer);

private:
    const smile::LogEvent& m_event;
};

pthread_mutex_t allLoggerMutex = PTHREAD_MUTEX_INITIALIZER;

WriteEvent::WriteEvent(const smile::LogEvent& event)
    : m_event(event)
{
}

void WriteEvent::operator() (smile::LogWriter*& writer)
{
    writer->write(m_event);
}

}

namespace smile
{

Logger::Logger(const std::string& name)
    : m_name(name), m_level(LogLevel_None)
{
    pthread_mutex_init(&m_mutex, 0);
}

Logger::~Logger()
{
    pthread_mutex_lock(&m_mutex);
    std::for_each(m_writers.begin(), m_writers.end(), DeletePointer<LogWriter>());
    pthread_mutex_unlock(&m_mutex);
    pthread_mutex_destroy(&m_mutex);
}

void Logger::addLogWriter(const LogWriter& writer)
{
    Locker lock(m_mutex);
    m_writers.push_back(writer.duplicate());
}

void Logger::force(LogLevel level, const std::string& message)
{
    LogEvent event(m_name, message, level);
    WriteEvent writeIt(event);
    Locker lock(m_mutex);
    std::for_each(m_writers.begin(), m_writers.end(), writeIt);
}

Logger& Logger::getLogger(const std::string& name)
{
    std::map<std::string, Logger*>& loggers =
        StaticInitializer::getInstance().getAllLoggers();
    Locker lock(allLoggerMutex);
    std::map<std::string, Logger*>::iterator found = loggers.find(name);
    if (found == loggers.end())
    {
        std::pair<std::map<std::string, Logger*>::iterator, bool> p =
            loggers.insert(std::make_pair(name, new Logger(name)));
        found = p.first;
    }
    return *found->second;
}

}
