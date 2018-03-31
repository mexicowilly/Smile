#include "Packet.hpp"
#include "ByteOrder.hpp"
#include <string.h>

namespace smile
{

Packet::Packet(size_t size)
    : m_bytes(new uint8_t[size]), m_size(size)
{
    assert(size >= 4);
    memset(m_bytes, 0, size);
    setInt32(0, size);
}

Packet::Packet(const Packet& packet)
    : m_bytes(new uint8_t[packet.getSize()]), m_size(packet.getSize())
{
    memcpy(m_bytes, packet.m_bytes, packet.getSize());
}

Packet::~Packet()
{
    delete [] m_bytes;
}

Packet& Packet::operator= (const Packet& packet)
{
    if (&packet != this)
    {
        delete [] m_bytes;
        size_t size = packet.getSize();
        m_bytes = new uint8_t[size];
        memcpy(m_bytes, packet.m_bytes, size);
    }
    return *this;
}

std::vector<uint8_t> Packet::getBytes(unsigned offset, size_t length) const
{
    if (length == 0)
        return std::vector<uint8_t>();
    assert(offset < getSize());
    assert(offset + length <= getSize());
    return std::vector<uint8_t>(m_bytes + offset, m_bytes + offset + length);
}

uint16_t Packet::getInt16(unsigned offset) const
{
    uint16_t value;
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    memcpy(&value, m_bytes + offset, sizeof(value));
    return N16_TO_H(value);
}

uint32_t Packet::getInt32(unsigned offset) const
{
    uint32_t value;
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    memcpy(&value, m_bytes + offset, sizeof(value));
    return N32_TO_H(value);
}

uint64_t Packet::getInt64(unsigned offset) const
{
    uint64_t value;
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    memcpy(&value, m_bytes + offset, sizeof(value));
    return N64_TO_H(value);
}

void Packet::read(Connection& connection)
{
    connection.read(m_bytes, getSize());
}

void Packet::setBytes(unsigned offset, const uint8_t* const bytes, size_t length)
{
    assert(length == 0 || offset < getSize());
    assert(offset + length <= getSize());
    memcpy(m_bytes + offset, bytes, length);
}

void Packet::setInt16(unsigned offset, uint16_t value)
{
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    uint16_t swapped = H16_TO_N(value);
    memcpy(m_bytes + offset, &swapped, sizeof(swapped));
}

void Packet::setInt32(unsigned offset, uint32_t value)
{
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    uint32_t swapped = H32_TO_N(value);
    memcpy(m_bytes + offset, &swapped, sizeof(swapped));
}

void Packet::setInt64(unsigned offset, uint64_t value)
{
    assert(offset < getSize());
    assert(offset + sizeof(value) <= getSize());
    uint64_t swapped = H64_TO_N(value);
    memcpy(m_bytes + offset, &swapped, sizeof(swapped));
}

void Packet::write(Connection& connection)
{
    connection.write(m_bytes, getSize());
}

}
