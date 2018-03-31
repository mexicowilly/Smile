#if !defined(SMILE_OS400DATASTRUCTURE_HPP__)
#define SMILE_OS400DATASTRUCTURE_HPP__

#include "smile/Text.hpp"

namespace smile
{

class OS400DataStructure
{
public:
    OS400DataStructure(const std::vector<uint8_t>& bytes);
    OS400DataStructure(const OS400DataStructure& data);

    OS400DataStructure& operator= (const OS400DataStructure& data);

    std::vector<uint8_t> getBytes(unsigned offset, size_t length) const;
    uint8_t getInt8(unsigned offset) const;
    uint16_t getInt16(unsigned offset) const;
    uint32_t getInt32(unsigned offset) const;
    uint64_t getInt64(unsigned offset) const;
    size_t getSize() const;
    Text getText(unsigned offset, size_t length, int ccsid) const;

private:
    std::vector<uint8_t> m_bytes;
};

inline size_t OS400DataStructure::getSize() const
{
    return m_bytes.size();
}

}

#endif
