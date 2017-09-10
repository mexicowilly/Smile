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

#include "output_packet.hpp"

namespace smile
{

output_packet::output_packet()
{
    skip(sizeof(std::uint32_t));
}

output_packet& output_packet::operator<<(std::uint16_t i)
{
    union
    {
        std::uint16_t i;
        std::uint8_t b;
    } u{boost::endian::native_to_big(i)};
    bytes_.insert(bytes_.end(), &u.b, (&u.b) + sizeof(std::uint16_t));
    return *this;
}

output_packet& output_packet::operator<<(std::uint32_t i)
{
    union
    {
        std::uint32_t i;
        std::uint8_t b;
    } u{boost::endian::native_to_big(i)};
    bytes_.insert(bytes_.end(), &u.b, (&u.b) + sizeof(std::uint32_t));
    return *this;
}

output_packet& output_packet::operator<<(std::uint64_t i)
{
    union
    {
        std::uint64_t i;
        std::uint8_t b;
    } u{boost::endian::native_to_big(i)};
    bytes_.insert(bytes_.end(), &u.b, (&u.b) + sizeof(std::uint64_t));
    return *this;
}

void output_packet::finish()
{
    std::uint32_t i = boost::endian::native_to_big(static_cast<uint32_t>(bytes_.size()));
    *(reinterpret_cast<std::uint32_t*>(&bytes_[0])) = i;
}

}
