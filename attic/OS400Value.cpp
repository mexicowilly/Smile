#include <smile/OS400Value.hpp>

namespace smile
{

template <>
Text OS400Value<Text>::toText() const
{
    return m_value;
}

template <>
Text OS400Value<std::vector<Text> >::toText() const
{
    Text space(SMILE_WSTR(" "));
    Text quote(SMILE_WSTR("'"));
    Text result(quote);
    for (std::vector<Text>::const_iterator itor = m_value.begin();
         itor != m_value.end();
         itor++)
    {
        result += *itor;
        result += space;
    }
    result += quote;
    return result;
}

}
