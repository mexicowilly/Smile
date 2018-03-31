#if !defined(SMILE_COMMANDTEST_HPP__)
#define SMILE_COMMANDTEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class CommandTest : public hominid::TestCase, public ServerTest
{
public:
    void testCommand();

    HOMINID_BEGIN_SUITE(CommandTest)
        HOMINID_TEST(testCommand)
    HOMINID_END_SUITE
};

#endif
