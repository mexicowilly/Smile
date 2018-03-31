#if !defined(SMILE_LOGEVENTFORMATTER_HPP__)
#define SMILE_LOGEVENTFORMATTER_HPP__

#include <smile/Duplicable.hpp>
#include <smile/LogEvent.hpp>

namespace smile
{

class LogEventFormatter : public Duplicable<LogEventFormatter>
{
public:
    virtual std::string format(const LogEvent& event) = 0;
    virtual const char* getLevelName(LogLevel level) const;
    virtual size_t getMaxLevelNameLength() const;
};

}

#endif
