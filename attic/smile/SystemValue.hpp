#if !defined(SMILE_SYSTEMVALUE_HPP__)
#define SMILE_SYSTEMVALUE_HPP__

#include <smile/OS400NameValuePair.hpp>

namespace smile
{

class SystemValue : public OS400NameValuePair
{
public:
    SystemValue(AS400& as400, const Text& name);
    SystemValue(const SystemValue& sysval);

    SystemValue& operator= (const SystemValue& sysval);

    virtual OS400NameValuePair* duplicate() const;

protected:
    virtual bool isSystemValue() const;

private:
    friend class SystemValueGroup;

    SystemValue(AS400& as400, const Text& name, OS400GenericValue* value);
};

inline SystemValue::SystemValue(AS400& as400, const Text& name)
    : OS400NameValuePair(as400, name)
{
}

inline SystemValue::SystemValue(const SystemValue& sysval)
    : OS400NameValuePair(sysval)
{
}

inline SystemValue& SystemValue::operator= (const SystemValue& sysval)
{
    OS400NameValuePair::operator=(sysval);
    return *this;
}

}

#endif
