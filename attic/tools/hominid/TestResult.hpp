#if !defined(HOMINID_TESTRESULT_HPP__)
#define HOMINID_TESTRESULT_HPP__

#include <map>
#include <list>
#include <vector>
#include <hominid/TestObserver.hpp>

namespace hominid
{

class Test;
class TestCase;

class TestResult
{
public:
    class TestMessagePair
    {
    public:
        TestMessagePair(const std::string& one, const std::string& two);
        TestMessagePair(const TestMessagePair& pair);

        std::string first;
        std::string second;
    };

    TestResult();

    void addObserver(TestObserver& observer);
    void errTest(const Test& test, const std::string& message);
    void failTest(const Test& test, const std::string& message);
    size_t getErrorCount() const;
    std::vector<TestMessagePair> getErrors() const;
    size_t getFailureCount() const;
    std::vector<TestMessagePair> getFailures() const;
    bool isClean() const;
    void removeObserver(TestObserver& observer);
    void run(TestCase& test);
    bool shouldStop() const;
    void stop();

private:
    std::multimap<std::string, std::string> m_failures;
    std::multimap<std::string, std::string> m_errors;
    std::list<TestObserver*> m_observers;
    bool m_quit;
};

inline void TestResult::addObserver(TestObserver& observer)
{
    m_observers.push_back(&observer);
}

inline size_t TestResult::getErrorCount() const
{
    return m_errors.size();
}

inline size_t TestResult::getFailureCount() const
{
    return m_failures.size();
}

inline bool TestResult::isClean() const
{
    return m_errors.size() == 0 && m_failures.size() == 0;
}

inline void TestResult::removeObserver(TestObserver& observer)
{
    m_observers.remove(&observer);
}

inline bool TestResult::shouldStop() const
{
    return m_quit;
}

inline void TestResult::stop()
{
    m_quit = true;
}

inline TestResult::TestMessagePair::TestMessagePair(const std::string& one, const std::string& two)
    : first(one), second(two)
{
}

inline TestResult::TestMessagePair::TestMessagePair(const TestMessagePair& pair)
    : first(pair.first), second(pair.second)
{
}

}

#endif
