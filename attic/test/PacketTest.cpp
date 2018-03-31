#include "PacketTest.hpp"
#include "AccessPacket.hpp"

void PacketTest::testAccessPacket()
{
    smile::AccessPacket packet(50);
    packet.setCorrelation(101);
    predicateEquals(HERE, 101U, packet.getCorrelation());
    packet.setCsInstance(102);
    predicateEquals(HERE, 102U, packet.getCsInstance());
    packet.setHeaderId(103);
    predicateEquals(HERE, static_cast<uint16_t>(103), packet.getHeaderId());
    packet.setRequestReplyId(104);
    predicateEquals(HERE, static_cast<uint16_t>(104), packet.getRequestReplyId());
    packet.setServerId(105);
    predicateEquals(HERE, static_cast<uint16_t>(105), packet.getServerId());
    packet.setTemplateLength(106);
    predicateEquals(HERE, static_cast<uint16_t>(106), packet.getTemplateLength());
}

void PacketTest::testPacket()
{
    smile::Packet packet(50);
    predicateEquals(HERE, 50U, packet.getInt32(0));
    for (int i = 4; i < 50; i++)
        predicateEquals(HERE, static_cast<uint8_t>(0), packet.getInt8(i));
    packet.setInt8(1, 1);
    predicateEquals(HERE, static_cast<uint8_t>(1), packet.getInt8(1));
    packet.setInt16(2, 2);
    predicateEquals(HERE, static_cast<uint16_t>(2), packet.getInt16(2));
    packet.setInt32(4, 4);
    predicateEquals(HERE, static_cast<uint32_t>(4), packet.getInt32(4));
    packet.setInt64(8, 8);
    predicateEquals(HERE, static_cast<uint64_t>(8), packet.getInt64(8));
}
