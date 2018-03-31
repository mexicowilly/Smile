#include <hominid/TestCase.hpp>
#include <hominid/AssertionFailure.hpp>
#include <hominid/TestSuite.hpp>

namespace hominid
{

TestCase::TestCase(const std::string& name)
    : Test(name), m_hasNext(true)
{
}

int TestCase::getTestCount() const
{
    return 1;
}

void TestCase::fail(const char* const file, int line, const std::string& message)
{
    throw AssertionFailure(file, line, message);
}

bool TestCase::hasNext() const
{
    return m_hasNext;
}

Test& TestCase::next()
{
    m_hasNext = false;
    return *this;
}

void TestCase::predicate(const char* const file,
                         int line,
                         bool condition,
                         const std::string& message)
{
    if (!condition)
        fail(file, line, message);
}

void TestCase::reset()
{
    m_hasNext = true;
}

void TestCase::run(TestResult& result)
{
    result.run(*this);
}

void TestCase::runBare()
{
    setUp();
    try
    {
        runTest();
    }
    catch (...)
    {
        tearDown();
        throw;
    }
    tearDown();
}

void TestCase::setUp()
{
}

void TestCase::tearDown()
{
}

}
