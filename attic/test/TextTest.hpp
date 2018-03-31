#if !defined(SMILE_TEST_TEXTTEST_HPP__)
#define SMILE_TEST_TEXTTEST_HPP__

#include <hominid/TestCase.hpp>

class TextTest : public hominid::TestCase
{
public:
    void testAccess();
    void testCompare();
    void testConvert();
    void testCopy();
    void testFind();
    void testSubText();
    void testTokenize();
    void testToUppercase();
    void testTrim();

    HOMINID_BEGIN_SUITE(TextTest)
        HOMINID_TEST(testAccess)
        HOMINID_TEST(testCompare)
        HOMINID_TEST(testConvert)
        HOMINID_TEST(testCopy)
        HOMINID_TEST(testFind)
        HOMINID_TEST(testSubText)
        HOMINID_TEST(testTokenize)
        HOMINID_TEST(testToUppercase)
        HOMINID_TEST(testTrim)
    HOMINID_END_SUITE
};

#endif
