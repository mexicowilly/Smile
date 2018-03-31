#if !defined(SMILE_PACKET_HPP__)
#define SMILE_PACKET_HPP__

#include "Connection.hpp"
#include <sys/types.h>
#include <inttypes.h>
#include <assert.h>
#include <vector>

namespace smile
{

class Packet
{
public:
    Packet(size_t size);
    Packet(const Packet& packet);
    virtual ~Packet();

    Packet& operator= (const Packet& packet);

    std::vector<uint8_t> getBytes(unsigned offset, size_t length) const;
    uint8_t getInt8(unsigned offset) const;
    uint16_t getInt16(unsigned offset) const;
    uint32_t getInt32(unsigned offset) const;
    uint64_t getInt64(unsigned offset) const;
    size_t getSize() const;
    virtual void read(Connection& connection);
    void setBytes(unsigned offset, const uint8_t* const bytes, size_t length);
    void setBytes(unsigned offset, const std::vector<uint8_t>& bytes);
    void setInt8(unsigned offset, uint8_t value);
    void setInt16(unsigned offset, uint16_t value);
    void setInt32(unsigned offset, uint32_t value);
    void setInt64(unsigned offset, uint64_t value);
    virtual void write(Connection& connection);

protected:
    uint8_t* m_bytes;
    size_t m_size;
};

inline uint8_t Packet::getInt8(unsigned offset) const
{
    assert(offset < getSize());
    return m_bytes[offset];
}

inline void Packet::setInt8(unsigned offset, uint8_t value)
{
    assert(offset < getSize());
    m_bytes[offset] = value;
}

inline size_t Packet::getSize() const
{
    return m_size;
}

inline void Packet::setBytes(unsigned offset, const std::vector<uint8_t>& bytes)
{
    setBytes(offset, &bytes[0], bytes.size());
}

}

#endif
