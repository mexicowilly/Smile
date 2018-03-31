#include <smile/LogWriter.hpp>
#include <smile/SimpleLogEventFormatter.hpp>

namespace smile
{

LogWriter::LogWriter(const std::string& name)
    : m_name(name), m_formatter(new SimpleLogEventFormatter())
{
}

LogWriter::LogWriter(const std::string& name, const LogEventFormatter& formatter)
    : m_name(name), m_formatter(formatter.duplicate())
{
}

LogWriter::LogWriter(const LogWriter& writer)
    : m_name(writer.m_name),
      m_formatter((writer.m_formatter == 0) ? 0 : writer.m_formatter->duplicate())
{
}

LogWriter::~LogWriter()
{
    delete m_formatter;
}

LogWriter& LogWriter::operator= (const LogWriter& writer)
{
    if (&writer != this)
        m_name = writer.m_name;
    return *this;
}

void LogWriter::setFormatter(const LogEventFormatter& formatter)
{
    delete m_formatter;
    m_formatter = formatter.duplicate();
}

}
