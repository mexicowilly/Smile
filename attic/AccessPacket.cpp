#include "AccessPacket.hpp"
#include "ByteOrder.hpp"

namespace smile
{

void AccessPacket::read(Connection& connection)
{
    connection.read(m_bytes + sizeof(uint32_t), m_size - sizeof(uint32_t));
}

}
