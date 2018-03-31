#if !defined(SMILE_CONSOLELOGWRITER_HPP__)
#define SMILE_CONSOLELOGWRITER_HPP__

#include <smile/LogWriter.hpp>

namespace smile
{

class ConsoleLogWriter : public LogWriter
{
public:
    ConsoleLogWriter();
    ConsoleLogWriter(const ConsoleLogWriter& writer);

    ConsoleLogWriter& operator= (const ConsoleLogWriter& writer);

    virtual LogWriter* duplicate() const;
    virtual void write(const LogEvent& event);
};

}

#endif
