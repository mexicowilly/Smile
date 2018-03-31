#if !defined(SMILE_OS400NAMEVALUEPAIR_HPP__)
#define SMILE_OS400NAMEVALUEPAIR_HPP__

#include <smile/AS400.hpp>
#include <smile/OS400Value.hpp>

namespace smile
{

class OS400NameValuePair : public Duplicable<OS400NameValuePair>
{
public:
    ~OS400NameValuePair();

    bool operator< (const OS400NameValuePair& pair) const;
    bool operator> (const OS400NameValuePair& pari) const;

    AS400& getAS400() const;
    const Text& getName() const;
    const OS400GenericValue& getValue();
    void refresh();
    void setValue(const OS400GenericValue& value);

protected:
    OS400NameValuePair(AS400& as400, const Text& name);
    OS400NameValuePair(const OS400NameValuePair& pair);

    OS400NameValuePair& operator= (const OS400NameValuePair& pair);

    virtual bool isSystemValue() const = 0;
    void setValueLocally(OS400GenericValue* value);

private:
    AS400* m_as400;
    Text m_name;
    OS400GenericValue* m_value;
};

inline bool OS400NameValuePair::operator< (const OS400NameValuePair& pair) const
{
    return m_name < pair.m_name;
}

inline bool OS400NameValuePair::operator> (const OS400NameValuePair& pair) const
{
    return m_name > pair.m_name;
}

inline AS400& OS400NameValuePair::getAS400() const
{
    return *m_as400;
}

inline const Text& OS400NameValuePair::getName() const
{
    return m_name;
}

}

#endif
