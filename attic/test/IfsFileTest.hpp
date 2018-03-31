#if !defined(SMILE_TEST_IFSFILETEST_HPP__)
#define SMILE_TEST_IFSFILETEST_HPP__

#include "ServerTest.hpp"
#include <hominid/TestCase.hpp>

class IfsFileTest : public hominid::TestCase, public ServerTest
{
public:
    void testDuplicate();
    void testExists();
    void testGetAttributes();
    void testRemove();
    void testSetAttributes();

    HOMINID_BEGIN_SUITE(IfsFileTest)
        HOMINID_TEST(testDuplicate)
        HOMINID_TEST(testExists)
        HOMINID_TEST(testGetAttributes)
        HOMINID_TEST(testRemove)
        HOMINID_TEST(testSetAttributes)
    HOMINID_END_SUITE
};

#endif
