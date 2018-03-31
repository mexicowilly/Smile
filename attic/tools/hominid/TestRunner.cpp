#include <hominid/TestRunner.hpp>
#include <Config.hpp>
// Some system have gettimeofday() in time.h and other in sys/time.h.
// Just include both.
#include <time.h>
#include <sys/time.h>
#include <typeinfo>
#include <sstream>
#if defined(HOMINID_GCC_DEMANGLING)
#include <cxxabi.h>
#include <assert.h>
#endif

namespace
{

const char* DASHES = "------------------------------------------------------------------------";
const char* EQUALS = "========================================================================";

void timevalDiff(struct timeval& first, struct timeval& second, struct timeval& result)
{
    if (first.tv_usec < second.tv_usec)
    {
        int nsec = (second.tv_usec - first.tv_usec) / 1000000 + 1;
        second.tv_usec -= 1000000 * nsec;
        second.tv_sec += nsec;
    }
    if (first.tv_usec - second.tv_usec > 1000000)
    {
        int nsec = (second.tv_usec - first.tv_usec) / 1000000;
        second.tv_usec += 1000000 * nsec;
        second.tv_sec -= nsec;
    }
    result.tv_sec = first.tv_sec - second.tv_sec;
    result.tv_usec = first.tv_usec - second.tv_usec;
}

std::string timevalToString(const struct timeval& val)
{
    std::ostringstream strm;
    strm.setf(std::ios_base::fixed, std::ios_base::floatfield);
    strm << static_cast<double>(val.tv_sec) + static_cast<double>(val.tv_usec) / 1000000.0;
    return strm.str();
}

std::string typeName(const std::type_info& ti)
{
#if defined(HOMINID_GCC_DEMANGLING)
    int status;
    char* demangled = HOMINID_CXA_NAMESPACE::__cxa_demangle(ti.name(), 0, 0, &status);
    assert(status != -1);
    assert(status != -2);
    assert(status != -3);
    assert(status == 0);
    std::string output(demangled);
    free(demangled);
    return output;
#else
    return std::string(ti.name());
#endif
}

}

namespace hominid
{

TestRunner::TestRunner(std::ostream& stream)
    : m_out(stream)
{
}

void TestRunner::printErrors(TestResult& result)
{
    m_out << result.getErrorCount()
          << std::string((result.getErrorCount() == 1) ? " error " : " errors ")
          << std::string("detected")
          << std::string((result.getErrorCount() == 0) ? "." : ":")
          << std::endl;
    std::vector<TestResult::TestMessagePair> errors = result.getErrors();
    for (std::vector<TestResult::TestMessagePair>::iterator itor = errors.begin();
         itor != errors.end();
         itor++)
    {
        m_out << "* " << itor->first << ": " << itor->second << std::endl;
    }
    if (result.getErrorCount() > 0)
        m_out << std::endl;
}

void TestRunner::printFailures(TestResult& result)
{
    m_out << result.getFailureCount()
          << std::string((result.getFailureCount() == 1) ? " failure " : " failures ")
          << std::string("detected")
          << std::string((result.getFailureCount() == 0) ? "." : ":")
          << std::endl;
    std::vector<TestResult::TestMessagePair> failures = result.getFailures();
    for (std::vector<TestResult::TestMessagePair>::iterator itor = failures.begin();
         itor != failures.end();
         itor++)
    {
        m_out << "* " << itor->first << ": " << itor->second << std::endl;
    }
}

void TestRunner::printHeader(const Test& test)
{
	time_t now = time(0);
	m_out << "Hominid Unit Testing Framework version 1.0" << std::endl;
	m_out << "Copyright (c) 2008, Will Mason. All rights reserved." << std::endl;
	m_out << std::endl;
	m_out << "Running \"" << test.getName() << "\" on " << ctime(&now) << std::endl;
}

void TestRunner::run(Test& test)
{
    struct timeval startTime;
    struct timeval endTime;
    struct timeval runTime;

	printHeader(test);

    TestResult result;
    result.addObserver(*this);
    gettimeofday(&startTime, 0);
    test.run(result);
    gettimeofday(&endTime, 0);
    timevalDiff(endTime, startTime, runTime);
    m_out << std::endl <<
        test.getTestCount() << " tests run in " <<
        timevalToString(runTime) << " seconds." << std::endl;
    printErrors(result);
    printFailures(result);
    m_out << std::endl;
}

void TestRunner::testEnded(const Test& test)
{
    m_out << DASHES << std::endl;
    m_out << "  END: " << typeName(typeid(test)) << "::" << test.getName() << std::endl;
    m_out << EQUALS << std::endl;
}

void TestRunner::testStarted(const Test& test)
{
    m_out << EQUALS << std::endl;
    m_out << "BEGIN: " << typeName(typeid(test)) << "::" << test.getName() << std::endl;
    m_out << DASHES << std::endl;
}

}
