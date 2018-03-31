#if !defined(SMILE_TEST_SERVICEPROGRAMPROCEDURETEST_HPP__)
#define SMILE_TEST_SERVICEPROGRAMPROCEDURETEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class ServiceProgramProcedureTest : public hominid::TestCase, public ServerTest
{
public:
    void testQp0lGetAttr();
    void testGettimeofday();

    HOMINID_BEGIN_SUITE(ServiceProgramProcedureTest)
        HOMINID_TEST(testQp0lGetAttr)
        HOMINID_TEST(testGettimeofday)
    HOMINID_END_SUITE
};

#endif
