#include "OS400DataStructure.hpp"
#include "ByteOrder.hpp"
#include <assert.h>

namespace smile
{

OS400DataStructure::OS400DataStructure(const std::vector<uint8_t>& bytes)
    : m_bytes(bytes)
{
}

OS400DataStructure::OS400DataStructure(const OS400DataStructure& data)
    : m_bytes(data.m_bytes)
{
}

OS400DataStructure& OS400DataStructure::operator= (const OS400DataStructure& data)
{
    m_bytes = data.m_bytes;
    return *this;
}

std::vector<uint8_t> OS400DataStructure::getBytes(unsigned offset, size_t length) const
{
    assert(offset < m_bytes.size());
    assert(offset + length <= m_bytes.size());
    return std::vector<uint8_t>(m_bytes.begin() + offset, m_bytes.begin() + offset + length);
}

uint8_t OS400DataStructure::getInt8(unsigned offset) const
{
    assert(offset < m_bytes.size());
    return m_bytes[offset];
}

uint16_t OS400DataStructure::getInt16(unsigned offset) const
{
    assert(offset < m_bytes.size());
    assert(offset + sizeof(uint16_t) <= m_bytes.size());
    uint16_t value;
    memcpy(&value, &m_bytes[offset], sizeof(value));
    return N16_TO_H(value);
}

uint32_t OS400DataStructure::getInt32(unsigned offset) const
{
    assert(offset < m_bytes.size());
    assert(offset + sizeof(uint32_t) <= m_bytes.size());
    uint32_t value;
    memcpy(&value, &m_bytes[offset], sizeof(value));
    return N32_TO_H(value);
}

uint64_t OS400DataStructure::getInt64(unsigned offset) const
{
    assert(offset < m_bytes.size());
    assert(offset + sizeof(uint64_t) <= m_bytes.size());
    uint64_t value;
    memcpy(&value, &m_bytes[offset], sizeof(value));
    return N64_TO_H(value);
}

Text OS400DataStructure::getText(unsigned offset, size_t length, int ccsid) const
{
    assert(offset < m_bytes.size());
    assert(offset + length <= m_bytes.size());
    return Text(&m_bytes[offset], length, ccsid);
}

}
