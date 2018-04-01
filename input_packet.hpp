#ifndef SMILE_INPUT_PACKET_HPP
#define SMILE_INPUT_PACKET_HPP

#include <boost/endian/conversion.hpp>

#include <cstdint>
#include <type_traits>
#include <stdexcept>
#include <vector>

namespace smile
{

class input_packet
{
public:
    static constexpr const std::size_t npos = std::numeric_limits<std::size_t>::max();

    // These bytes are not copied
    input_packet(const std::uint8_t* const bytes, std::size_t s);
    virtual ~input_packet();

    input_packet& operator>> (std::uint8_t& i);
    input_packet& operator>> (std::uint16_t& i);
    input_packet& operator>> (std::uint32_t& i);
    input_packet& operator>> (std::uint64_t& i);
    input_packet& operator>> (std::vector<std::uint8_t>& bytes);

    std::size_t find(std::size_t start, std::uint16_t code) const;
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type get(std::size_t offset) const;
    std::vector<std::uint8_t> get(std::size_t offset, std::size_t length) const;
    std::size_t size() const;
    void skip(std::size_t bytes);

private:
    const std::uint8_t* start_;
    const std::uint8_t* cur_;
    const std::uint8_t* end_;
};

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type input_packet::get(std::size_t offset) const
{
    auto this_one = start_ + offset;
    if (this_one > end_ - sizeof(T))
        throw std::out_of_range("Packet data request");
    return boost::endian::big_to_native(*(reinterpret_cast<const T*>(this_one)));
}

inline void input_packet::skip(std::size_t bytes)
{
    cur_ += std::min(bytes, static_cast<std::size_t>(end_ - cur_));
}

inline std::size_t input_packet::size() const
{
    return end_ - start_;
}

}

#endif
