#include <smile/OS400NameValuePair.hpp>
#include "OS400ValueManipulator.hpp"
#include <assert.h>

namespace smile
{

OS400NameValuePair::OS400NameValuePair(AS400& as400, const Text& name)
    : m_as400(&as400), m_name(name), m_value(0)
{
}

OS400NameValuePair::OS400NameValuePair(const OS400NameValuePair& pair)
    : m_as400(pair.m_as400),
      m_name(pair.m_name),
      m_value((pair.m_value == 0) ? 0 : pair.m_value->duplicate())
{
}

OS400NameValuePair::~OS400NameValuePair()
{
    delete m_value;
}

OS400NameValuePair& OS400NameValuePair::operator= (const OS400NameValuePair& pair)
{
    if (&pair != this)
    {
        m_as400 = pair.m_as400;
        m_name = pair.m_name;
        delete m_value;
        m_value = (pair.m_value == 0) ? 0 : pair.m_value->duplicate();
    }
    return *this;
}

const OS400GenericValue& OS400NameValuePair::getValue()
{
    if (m_value == 0)
        refresh();
    return *m_value;
}

void OS400NameValuePair::refresh()
{
    std::vector<Text> myName;
    myName.push_back(m_name);
    std::map<Text, OS400GenericValue*> myValue =
        OS400ValueManipulator::getInstance().retrieveValues(*m_as400, myName, isSystemValue());
    assert(myValue.find(m_name) != myValue.end());
    delete m_value;
    m_value = myValue[m_name];
}

void OS400NameValuePair::setValue(const OS400GenericValue& value)
{
    OS400ValueManipulator::getInstance().setValue(*m_as400, m_name, *m_value, isSystemValue());
}

void OS400NameValuePair::setValueLocally(OS400GenericValue* value)
{
    delete m_value;
    m_value = value;
}

}
