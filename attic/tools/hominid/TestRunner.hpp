#if !defined(HOMINID_TESTRUNNER_HPP__)
#define HOMINID_TESTRUNNER_HPP__

#include <ostream>
#include <hominid/TestResult.hpp>

namespace hominid
{

class TestRunner : public TestObserver
{
public:
    TestRunner(std::ostream& stream);

    virtual void run(Test& test);
    virtual void testEnded(const Test& test);
    virtual void testStarted(const Test& test);

protected:
    void printErrors(TestResult& result);
    void printFailures(TestResult& result);
	void printHeader(const Test& test);

private:
    std::ostream& m_out;
};

}

#endif
