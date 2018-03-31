#if !defined(SMILE_TEST_AS400IMPLTEST_HPP__)
#define SMILE_TEST_AS400IMPLTEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class AS400ImplTest : public hominid::TestCase, public ServerTest
{
public:
    virtual void setUp();
    virtual void tearDown();
    void testGetCCSID();
    void testGetPort();
    void testPrepareConnection();
    void testSignon();

    HOMINID_BEGIN_SUITE(AS400ImplTest)
        HOMINID_TEST(testGetCCSID)
        HOMINID_TEST(testGetPort)
        HOMINID_TEST(testPrepareConnection)
        HOMINID_TEST(testSignon)
    HOMINID_END_SUITE

private:
    smile::AS400* m_localAS400;
};

#endif
