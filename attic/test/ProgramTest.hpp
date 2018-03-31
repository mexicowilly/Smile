#if !defined(SMILE_TEST_PROGRAMTEST_HPP__)
#define SMILE_TEST_PROGRAMTEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class ProgramTest : public hominid::TestCase, public ServerTest
{
public:
    void testProgram();

    HOMINID_BEGIN_SUITE(ProgramTest)
        HOMINID_TEST(testProgram)
    HOMINID_END_SUITE
};

#endif
