#include <hominid/TestResult.hpp>
#include <hominid/AssertionFailure.hpp>
#include <hominid/TestCase.hpp>

namespace
{

const std::string UNKNOWN_OBJECT = "Unknown object thrown";

}

namespace hominid
{

TestResult::TestResult()
    : m_quit(false)
{
}

void TestResult::errTest(const Test& test, const std::string& message)
{
    m_errors.insert(std::pair<const std::string, std::string>(test.getName(), message));
    for (std::list<TestObserver*>::iterator itor = m_observers.begin();
         itor != m_observers.end();
         itor++)
    {
        (*itor)->errorAdded(test, message);
    }
}

void TestResult::failTest(const Test& test, const std::string& message)
{
    m_failures.insert(std::pair<const std::string, std::string>(test.getName(), message));
    for (std::list<TestObserver*>::iterator itor = m_observers.begin();
         itor != m_observers.end();
         itor++)
    {
        (*itor)->failureAdded(test, message);
    }
}

std::vector<TestResult::TestMessagePair> TestResult::getErrors() const
{
    std::vector<TestMessagePair> result;
    for (std::multimap<std::string, std::string>::const_iterator itor = m_errors.begin();
         itor != m_errors.end();
         itor++)
    {
        result.push_back(TestMessagePair(itor->first, itor->second));
    }
    return result;
}

std::vector<TestResult::TestMessagePair> TestResult::getFailures() const
{
    std::vector<TestMessagePair> result;
    for (std::multimap<std::string, std::string>::const_iterator itor = m_failures.begin();
         itor != m_failures.end();
         itor++)
    {
        result.push_back(TestMessagePair(itor->first, itor->second));
    }
    return result;
}

void TestResult::run(TestCase& test)
{
    for (std::list<TestObserver*>::iterator itor = m_observers.begin();
         itor != m_observers.end();
         itor++)
    {
        (*itor)->testStarted(test);
    }
    try
    {
        test.runBare();
    }
    catch (AssertionFailure& failure)
    {
        failTest(test, failure.toString());
    }
    catch (std::exception& except)
    {
        errTest(test, except.what());
    }
    catch (...)
    {
        errTest(test, UNKNOWN_OBJECT);
    }
    for (std::list<TestObserver*>::iterator itor = m_observers.begin();
         itor != m_observers.end();
         itor++)
    {
        (*itor)->testEnded(test);
    }
}

}
