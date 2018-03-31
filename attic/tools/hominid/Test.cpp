#include <hominid/Test.hpp>

namespace hominid
{

Test::Test(const std::string& name)
    : m_name(name)
{
}

Test::Test(const Test& test)
    : m_name(test.m_name)
{
}

Test::~Test()
{
}

Test& Test::operator=(const Test& test)
{
    if (this != &test)
    {
        m_name = test.m_name;
    }
    return *this;
}

}
