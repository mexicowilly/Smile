#if !defined(SMILE_TEST_CONNECTIONTEST_HPP__)
#define SMILE_TEST_CONNECTIONTEST_HPP__

#include <hominid/TestCase.hpp>

class ConnectionTest : public hominid::TestCase
{
public:
    void testConnect();

    HOMINID_BEGIN_SUITE(ConnectionTest)
        HOMINID_TEST(testConnect)
    HOMINID_END_SUITE
};

#endif
