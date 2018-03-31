#if !defined(SMILE_TEST_TEXTCONVERTERTEST_HPP__)
#define SMILE_TEST_TEXTCONVERTERTEST_HPP__

#include <hominid/TestCase.hpp>

class TextConverterTest : public hominid::TestCase
{
public:
    void testEbcdicToUtf8();

    HOMINID_BEGIN_SUITE(TextConverterTest)
        HOMINID_TEST(testEbcdicToUtf8)
    HOMINID_END_SUITE
};

#endif
