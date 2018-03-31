#if !defined(SMILE_TEST_PACKETTEST_HPP__)
#define SMILE_TEST_PACKETTEST_HPP__

#include <hominid/TestCase.hpp>

class PacketTest : public hominid::TestCase
{
public:
    void testAccessPacket();
    void testPacket();

    HOMINID_BEGIN_SUITE(PacketTest)
        HOMINID_TEST(testAccessPacket)
        HOMINID_TEST(testPacket)
    HOMINID_END_SUITE
};

#endif
