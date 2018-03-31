#if !defined(SMILE_TEST_IFSFILEHANDLETEST_HPP__)
#define SMILE_TEST_IFSFILEHANDLETEST_HPP__

#include <hominid/TestCase.hpp>
#include "ServerTest.hpp"
#include <smile/IfsFileHandle.hpp>

class IfsFileHandleTest : public hominid::TestCase, public ServerTest
{
public:
    virtual void setUp();
    virtual void tearDown();
    void testOpenClose();
    void testReadWrite();
    void testSeekTell();

    HOMINID_BEGIN_SUITE(IfsFileHandleTest)
        HOMINID_TEST(testOpenClose)
        HOMINID_TEST(testReadWrite)
        HOMINID_TEST(testSeekTell)
    HOMINID_END_SUITE

private:
    smile::IfsFile* m_file;
    smile::IfsFileHandle* m_handle;
};

#endif
