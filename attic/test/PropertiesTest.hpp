#if !defined(SMILE_TEST_PROPERTIESTEST_HPP__)
#define SMILE_TEST_PROPERTIESTEST_HPP__

#include <hominid/TestCase.hpp>

class PropertiesTest : public hominid::TestCase
{
public:
    void testAccessors();
    void testComparing();
    void testCopying();
    void testFileLoading();
    void testMerge();

    HOMINID_BEGIN_SUITE(PropertiesTest)
        HOMINID_TEST(testAccessors)
        HOMINID_TEST(testComparing)
        HOMINID_TEST(testCopying)
        HOMINID_TEST(testFileLoading)
        HOMINID_TEST(testMerge)
    HOMINID_END_SUITE
};

#endif
