#if !defined(SMILE_LOGGER_HPP__)
#define SMILE_LOGGER_HPP__

#include <smile/LogWriter.hpp>
#include <list>
#include <pthread.h>

#define SMILE_LOG_FORCE(level, logger, message) \
    do \
    { \
        std::ostringstream logMacroStream; \
        logMacroStream << message; \
        logger.force(level, logMacroStream.str()); \
    } while (false)

#define SMILE_LOG_BASE(level, logger, message) \
    do \
    { \
        if (logger.is##level##Enabled()) \
        { \
            SMILE_LOG_FORCE(::smile::LogLevel_##level, logger, message); \
        } \
    } while (false)

#define SMILE_LOG_DEBUG(logger, message) SMILE_LOG_BASE(Debug, logger, message)
#define SMILE_LOG_ERROR(logger, message) SMILE_LOG_BASE(Error, logger, message)
#define SMILE_LOG_FATAL(logger, message) SMILE_LOG_BASE(Fatal, logger, message)
#define SMILE_LOG_INFO(logger, message) SMILE_LOG_BASE(Info, logger, message)
#define SMILE_LOG_TRACE(logger, message) SMILE_LOG_BASE(Trace, logger, message)
#define SMILE_LOG_WARN(logger, message) SMILE_LOG_BASE(Warn, logger, message)

namespace smile
{

class Logger
{
public:
    static Logger& getLogger(const std::string& name);

    ~Logger();

    void addLogWriter(const LogWriter& writer);
    void debug(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
    void force(LogLevel level, const std::string& message);
    LogLevel getLogLevel() const;
    const std::string& getName() const;
    void info(const std::string& message);
    bool isDebugEnabled() const;
    bool isEnabled(LogLevel level) const;
    bool isErrorEnabled() const;
    bool isFatalEnabled() const;
    bool isInfoEnabled() const;
    bool isTraceEnabled() const;
    bool isWarnEnabled() const;
    void log(LogLevel level, const std::string& message);
    void setLogLevel(LogLevel level);
    void trace(const std::string& message);
    void warn(const std::string& message);

private:
    Logger(const std::string& name);
    Logger(const Logger&);
    Logger& operator= (Logger&);

    std::string m_name;
    std::list<LogWriter*> m_writers;
    LogLevel m_level;
    pthread_mutex_t m_mutex;
};

inline void Logger::debug(const std::string& message)
{
    log(LogLevel_Debug, message);
}

inline void Logger::error(const std::string& message)
{
    log(LogLevel_Error, message);
}

inline void Logger::fatal(const std::string& message)
{
    log(LogLevel_Fatal, message);
}

inline LogLevel Logger::getLogLevel() const
{
    return m_level;
}

inline const std::string& Logger::getName() const
{
    return m_name;
}

inline void Logger::info(const std::string& message)
{
    log(LogLevel_Info, message);
}

inline bool Logger::isDebugEnabled() const
{
    return isEnabled(LogLevel_Debug);
}

inline bool Logger::isEnabled(LogLevel level) const
{
    return m_level <= level;
}

inline bool Logger::isErrorEnabled() const
{
    return isEnabled(LogLevel_Error);
}

inline bool Logger::isFatalEnabled() const
{
    return isEnabled(LogLevel_Fatal);
}

inline bool Logger::isInfoEnabled() const
{
    return isEnabled(LogLevel_Info);
}

inline bool Logger::isTraceEnabled() const
{
    return isEnabled(LogLevel_Trace);
}

inline bool Logger::isWarnEnabled() const
{
    return isEnabled(LogLevel_Warn);
}

inline void Logger::log(LogLevel level, const std::string& message)
{
    if (isEnabled(level))
        force(level, message);
}

inline void Logger::setLogLevel(LogLevel level)
{
    m_level = level;
}

inline void Logger::trace(const std::string& message)
{
    log(LogLevel_Trace, message);
}

inline void Logger::warn(const std::string& message)
{
    log(LogLevel_Warn, message);
}

}

#endif
