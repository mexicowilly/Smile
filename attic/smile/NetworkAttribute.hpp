#if !defined(SMILE_NETWORKATTRIBUTE_HPP__)
#define SMILE_NETWORKATTRIBUTE_HPP__

#include <smile/OS400NameValuePair.hpp>

namespace smile
{

class NetworkAttribute : public OS400NameValuePair
{
public:
    static std::vector<NetworkAttribute> getAll(AS400& as400);

    NetworkAttribute(AS400& as400, const Text& name);
    NetworkAttribute(const NetworkAttribute& sysval);

    NetworkAttribute& operator= (const NetworkAttribute& sysval);

    virtual OS400NameValuePair* duplicate() const;

protected:
    virtual bool isSystemValue() const;

private:
    NetworkAttribute(AS400& as400, const Text& name, OS400GenericValue* value);
};

inline NetworkAttribute::NetworkAttribute(AS400& as400, const Text& name)
    : OS400NameValuePair(as400, name)
{
}

inline NetworkAttribute::NetworkAttribute(const NetworkAttribute& sysval)
    : OS400NameValuePair(sysval)
{
}

inline NetworkAttribute& NetworkAttribute::operator= (const NetworkAttribute& sysval)
{
    OS400NameValuePair::operator=(sysval);
    return *this;
}

}

#endif
