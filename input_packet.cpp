#include "input_packet.hpp"

namespace smile
{

input_packet::input_packet(const std::uint8_t* const bytes, std::size_t s)
    : start_(bytes),
      cur_(bytes),
      end_(bytes + s)
{
}

input_packet::~input_packet()
{
}

input_packet& input_packet::operator>> (std::uint8_t& i)
{
    if (cur_ >= end_)
        throw std::out_of_range("No more packet data");
    i = *cur_++;
    return *this;
}

input_packet& input_packet::operator>> (std::uint16_t& i)
{
    if (cur_ > end_ - sizeof(i))
        throw std::out_of_range("No more packet data");
    i = boost::endian::big_to_native(*(reinterpret_cast<const std::uint16_t*>(cur_)));
    cur_ += sizeof(i);
    return *this;
}

input_packet& input_packet::operator>> (std::uint32_t& i)
{
    if (cur_ > end_ - sizeof(i))
        throw std::out_of_range("No more packet data");
    i = boost::endian::big_to_native(*(reinterpret_cast<const std::uint32_t*>(cur_)));
    cur_ += sizeof(i);
    return *this;
}

input_packet& input_packet::operator>> (std::uint64_t& i)
{
    if (cur_ > end_ - sizeof(i))
        throw std::out_of_range("No more packet data");
    i = boost::endian::big_to_native(*(reinterpret_cast<const std::uint64_t*>(cur_)));
    cur_ += sizeof(i);
    return *this;
}

input_packet& input_packet::operator>> (std::vector<std::uint8_t>& bytes)
{
    if (cur_ > end_ - bytes.size())
        throw std::out_of_range("No more packet data");
    std::copy(cur_, cur_ + bytes.size(), bytes.begin());
    cur_ += bytes.size();
    return *this;
}

std::size_t input_packet::find(std::size_t start, std::uint16_t code) const
{
    for (std::size_t offset = start; offset < size(); offset += get<uint32_t>(offset))
    {
        if (get<std::uint16_t>(offset + 4) == code)
            return offset;
    }
    return npos;
}

std::vector<std::uint8_t> input_packet::get(std::size_t offset, std::size_t length) const
{
    auto this_one = start_ + offset;
    if (this_one > end_ - length)
        throw std::out_of_range("Packet data request");
    return std::vector<std::uint8_t>(this_one, this_one + length);
}

}