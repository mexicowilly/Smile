#if !defined(SMILE_SIMPLELOGEVENTFORMATTER_HPP__)
#define SMILE_SIMPLELOGEVENTFORMATTER_HPP__

#include <smile/LogEventFormatter.hpp>

namespace smile
{

class SimpleLogEventFormatter : public LogEventFormatter
{
public:
    SimpleLogEventFormatter();

    virtual LogEventFormatter* duplicate() const;
    virtual std::string format(const LogEvent& event);

private:
    SimpleLogEventFormatter(const SimpleLogEventFormatter&);
    SimpleLogEventFormatter& operator= (const SimpleLogEventFormatter&);
};

}

#endif
