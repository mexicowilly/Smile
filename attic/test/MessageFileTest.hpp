#if !defined(SMILE_TEST_MESSAGEFILETEST_HPP__)
#define SMILE_TEST_MESSAGEFILETEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class MessageFileTest : public hominid::TestCase, public ServerTest
{
public:
    void testBareMessage();

    HOMINID_BEGIN_SUITE(MessageFileTest)
        HOMINID_TEST(testBareMessage)
    HOMINID_END_SUITE
};

#endif
