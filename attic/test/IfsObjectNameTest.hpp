#if !defined(SMILE_TEST_IFSOBJECTNAMETEST_HPP__)
#define SMILE_TEST_IFSOBJECTNAMETEST_HPP__

#include <hominid/TestCase.hpp>

class IfsObjectNameTest : public hominid::TestCase
{
public:
    void testCompare();
    void testConstruct();
    void testGetObjectName();
    void testGetParentName();

    HOMINID_BEGIN_SUITE(IfsObjectNameTest)
        HOMINID_TEST(testCompare)
        HOMINID_TEST(testConstruct)
        HOMINID_TEST(testGetObjectName)
        HOMINID_TEST(testGetParentName)
    HOMINID_END_SUITE
};

#endif
