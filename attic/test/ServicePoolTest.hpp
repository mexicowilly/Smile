#if !defined(SMILE_TEST_SERVICEPOOLTEST_HPP__)
#define SMILE_TEST_SERVICEPOOLTEST_HPP__

#include <hominid/TestCase.hpp>

class ServicePoolTest : public hominid::TestCase
{
public:
    virtual void setUp();
    virtual void tearDown();
    void testMultiThreadOverflow();
    void testMultiThreadReuse();
    void testSingleThreadOverflow();
    void testSingleThreadReuse();

    HOMINID_BEGIN_SUITE(ServicePoolTest)
        HOMINID_TEST(testMultiThreadOverflow)
        HOMINID_TEST(testMultiThreadReuse)
        HOMINID_TEST(testSingleThreadOverflow)
        HOMINID_TEST(testSingleThreadReuse)
    HOMINID_END_SUITE

private:
    bool m_originalState;
};

#endif
