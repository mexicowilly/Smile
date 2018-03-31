#if !defined(SMILE_LOGWRITER_HPP__)
#define SMILE_LOGWRITER_HPP__

#include <smile/LogEventFormatter.hpp>

namespace smile
{

class LogWriter : public Duplicable<LogWriter>
{
public:
    ~LogWriter();

    const std::string& getName() const;
    void setFormatter(const LogEventFormatter& formatter);
    virtual void write(const LogEvent& event) = 0;

protected:
    LogWriter(const std::string& name);
    LogWriter(const std::string& name, const LogEventFormatter& formatter);
    LogWriter(const LogWriter& writer);

    LogWriter& operator= (const LogWriter& writer);

    LogEventFormatter* getFormatter();

private:
    std::string m_name;
    LogEventFormatter* m_formatter;
};

inline LogEventFormatter* LogWriter::getFormatter()
{
    return m_formatter;
}

inline const std::string& LogWriter::getName() const
{
    return m_name;
}

}

#endif
