#if !defined(SMILE_TEST_OS400VERSIONTEST_HPP__)
#define SMILE_TEST_OS400VERSIONTEST_HPP__

#include <hominid/TestCase.hpp>

class OS400VersionTest : public hominid::TestCase
{
public:
    void testOS400Version();

    HOMINID_BEGIN_SUITE(OS400VersionTest)
        HOMINID_TEST(testOS400Version)
    HOMINID_END_SUITE
};

#endif
