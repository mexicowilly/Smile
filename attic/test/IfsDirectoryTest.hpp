#if !defined(SMILE_TEST_IFSDIRECTORYTEST_HPP__)
#define SMILE_TEST_IFSDIRECTORYTEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class IfsDirectoryTest : public hominid::TestCase, public ServerTest
{
public:
    void testCreateAndRemove();
    void testDuplicate();
    void testExists();
    void testGetAttributes();
    void testListNames();
    void testListObjects();

    HOMINID_BEGIN_SUITE(IfsDirectoryTest)
        HOMINID_TEST(testCreateAndRemove)
        HOMINID_TEST(testDuplicate)
        HOMINID_TEST(testExists)
        HOMINID_TEST(testGetAttributes)
        HOMINID_TEST(testListNames)
        HOMINID_TEST(testListObjects)
    HOMINID_END_SUITE
};

#endif
