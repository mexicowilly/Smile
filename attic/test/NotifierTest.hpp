#if !defined(SMILE_TEST_NOTIFIERTEST_HPP__)
#define SMILE_TEST_NOTIFIERTEST_HPP__

#include <hominid/TestCase.hpp>

class NotifierTest : public hominid::TestCase
{
public:
    void testAnySender();
    void testData();
    void testName();
    void testNameAndSender();
    void testSender();

    HOMINID_BEGIN_SUITE(NotifierTest)
        HOMINID_TEST(testAnySender)
        HOMINID_TEST(testData)
        HOMINID_TEST(testName)
        HOMINID_TEST(testNameAndSender)
        HOMINID_TEST(testSender)
    HOMINID_END_SUITE
};

#endif
