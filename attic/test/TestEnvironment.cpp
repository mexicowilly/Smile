#include "TestEnvironment.hpp"

TestEnvironment::TestEnvironment()
{
    std::string fileName;
    char* srcdir = getenv("srcdir");
    if (srcdir != 0)
    {
        fileName = srcdir;
        fileName += "/";
    }
    fileName += "test.properties";
    m_properties = new Properties(fileName);
}

TestEnvironment::~TestEnvironment()
{
    delete m_properties;
}

TestEnvironment& TestEnvironment::getInstance()
{
    static TestEnvironment testEnvironment;
    return testEnvironment;
}
