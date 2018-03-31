#include <smile/ConsoleLogWriter.hpp>
#include <iostream>

namespace smile
{

ConsoleLogWriter::ConsoleLogWriter()
    : LogWriter("console")
{
}

ConsoleLogWriter::ConsoleLogWriter(const ConsoleLogWriter& writer)
    : LogWriter(writer)
{
}

ConsoleLogWriter& ConsoleLogWriter::operator= (const ConsoleLogWriter& writer)
{
    LogWriter::operator=(writer);
    return *this;
}

LogWriter* ConsoleLogWriter::duplicate() const
{
    return new ConsoleLogWriter(*this);
}

void ConsoleLogWriter::write(const LogEvent& event)
{
    LogEventFormatter* formatter = getFormatter();
    if (formatter != 0)
        std::cout << formatter->format(event) << std::endl;
}

}
