#include <smile/SystemValue.hpp>

namespace smile
{

SystemValue::SystemValue(AS400& as400, const Text& name, OS400GenericValue* value)
    : OS400NameValuePair(as400, name)
{
    setValueLocally(value);
}

OS400NameValuePair* SystemValue::duplicate() const
{
    return new SystemValue(*this);
}

bool SystemValue::isSystemValue() const
{
    return true;
}

}
