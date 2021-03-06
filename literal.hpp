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

#ifndef SMILE_LITERAL_HPP
#define SMILE_LITERAL_HPP

#include <cstdint>

namespace smile
{

constexpr std::uint8_t operator"" _u8(unsigned long long i)
{
    return static_cast<std::uint8_t>(i);
}

constexpr std::uint16_t operator"" _u16(unsigned long long i)
{
    return static_cast<std::uint16_t>(i);
}

constexpr std::uint32_t operator"" _u32(unsigned long long i)
{
    return static_cast<std::uint32_t>(i);
}

constexpr std::uint64_t operator"" _u64(unsigned long long i)
{
    return static_cast<std::uint64_t>(i);
}

}

#endif
