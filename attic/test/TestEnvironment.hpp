#if !defined(SMILE_TEST_TESTENVIRONMENT_HPP__)
#define SMILE_TEST_TESTENVIRONMENT_HPP__

#include "Properties.hpp"

class TestEnvironment
{
public:
    static TestEnvironment& getInstance();

    ~TestEnvironment();

    Properties& getProperties();

private:
    TestEnvironment();
    TestEnvironment(const TestEnvironment&);
    TestEnvironment& operator= (const TestEnvironment&);

    Properties* m_properties;
};

inline Properties& TestEnvironment::getProperties()
{
    return *m_properties;
}

#endif
