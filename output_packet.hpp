/*
 * Copyright 2017 Will Mason
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef SMILE_OUTPUT_PACKET_HPP
#define SMILE_OUTPUT_PACKET_HPP

#include <boost/endian/conversion.hpp>
#include <vector>
#include <cstdint>

namespace smile
{

class output_packet
{
public:
    output_packet();
    virtual ~output_packet();

    output_packet& operator<< (std::uint8_t i);
    output_packet& operator<< (std::uint16_t i);
    output_packet& operator<< (std::uint32_t i);
    output_packet& operator<< (std::uint64_t i);
    output_packet& operator<< (const std::vector<std::uint8_t>& bytes);

    const std::vector<std::uint8_t>& bytes() const;
    void finish();
    template<typename T>
    typename std::enable_if<std::is_integral<T>::value>::type set(std::size_t offset, T val)
    {
        std::size_t new_end = offset + sizeof(val);
        if (new_end >= bytes_.size())
            bytes_.resize(new_end, 0);
        T i = boost::endian::native_to_big(static_cast<uint32_t>(val));
        *(reinterpret_cast<T*>(&bytes_[offset])) = i;
    }
    void skip(std::size_t bytes);

private:
    std::vector<std::uint8_t> bytes_;
};

inline output_packet& output_packet::operator<< (std::uint8_t i)
{
    bytes_.push_back(i);
    return *this;
}

inline output_packet& output_packet::operator<< (const std::vector<std::uint8_t>& bytes)
{
    bytes_.insert(bytes_.end(), bytes.begin(), bytes.end());
    return *this;
}

inline const std::vector<std::uint8_t>& output_packet::bytes() const
{
    return bytes_;
}

inline void output_packet::skip(std::size_t bytes)
{
    bytes_.insert(bytes_.end(), bytes, 0);
}

}


#endif