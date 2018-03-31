#if !defined(SMILE_OS400VALUE_HPP__)
#define SMILE_OS400VALUE_HPP__

#include <smile/Duplicable.hpp>
#include <smile/Text.hpp>
#include <sstream>

namespace smile
{

class OS400GenericValue : public Duplicable<OS400GenericValue>
{
public:
    virtual Text toText() const = 0;
};

template <class T>
class OS400Value : public OS400GenericValue
{
public:
    OS400Value(const T& value);
    OS400Value(const OS400Value& value);

    OS400Value& operator= (const OS400Value& value);

    virtual OS400GenericValue* duplicate() const;
    const T& getValue() const;
    virtual Text toText() const;

private:
    T m_value;
};

template <class T>
OS400Value<T>::OS400Value(const T& value)
    : m_value(value)
{
}

template <class T>
OS400Value<T>::OS400Value(const OS400Value& value)
    : m_value(value.m_value)
{
}

template <class T>
OS400Value<T>& OS400Value<T>::operator= (const OS400Value& value)
{
    if (&value != this)
        m_value = value.m_value;
    return *this;
}

template <class T>
OS400GenericValue* OS400Value<T>::duplicate() const
{
    return new OS400Value(*this);
}

template <class T>
const T& OS400Value<T>::getValue() const
{
    return m_value;
}

template <class T>
Text OS400Value<T>::toText() const
{
    std::ostringstream stream;
    stream << m_value;
    return Text(stream.str().c_str());
}

template <>
Text OS400Value<Text>::toText() const;

template <>
Text OS400Value<std::vector<Text> >::toText() const;

}

#endif
