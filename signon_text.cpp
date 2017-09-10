#include "signon_text.hpp"

#include <map>
#include <stdexcept>

namespace
{

std::map<std::uint8_t, UChar> ebcdic_to_uchar
    {
        { 0x40, 0x0020 },
        { 0x5b, 0x0024 },
        { 0x6d, 0x005f },
        { 0x7b, 0x0023 },
        { 0x7c, 0x0040 },
        { 0xc1, 0x0041 },
        { 0xc2, 0x0042 },
        { 0xc3, 0x0043 },
        { 0xc4, 0x0044 },
        { 0xc5, 0x0045 },
        { 0xc6, 0x0046 },
        { 0xc7, 0x0047 },
        { 0xc8, 0x0048 },
        { 0xc9, 0x0049 },
        { 0xd1, 0x004a },
        { 0xd2, 0x004b },
        { 0xd3, 0x004c },
        { 0xd4, 0x004d },
        { 0xd5, 0x004e },
        { 0xd6, 0x004f },
        { 0xd7, 0x0050 },
        { 0xd8, 0x0051 },
        { 0xd9, 0x0052 },
        { 0xe2, 0x0053 },
        { 0xe3, 0x0054 },
        { 0xe4, 0x0055 },
        { 0xe5, 0x0056 },
        { 0xe6, 0x0057 },
        { 0xe7, 0x0058 },
        { 0xe8, 0x0059 },
        { 0xe9, 0x005a },
        { 0xf0, 0x0030 },
        { 0xf1, 0x0031 },
        { 0xf2, 0x0032 },
        { 0xf3, 0x0033 },
        { 0xf4, 0x0034 },
        { 0xf5, 0x0035 },
        { 0xf6, 0x0036 },
        { 0xf7, 0x0037 },
        { 0xf8, 0x0038 },
        { 0xf9, 0x0039 }
    };

std::map<UChar, std::uint8_t> uchar_to_ebcdic
    {
        { 0x0023, 0x7b },
        { 0x0024, 0x5b },
        { 0x0030, 0xf0 },
        { 0x0031, 0xf1 },
        { 0x0032, 0xf2 },
        { 0x0033, 0xf3 },
        { 0x0034, 0xf4 },
        { 0x0035, 0xf5 },
        { 0x0036, 0xf6 },
        { 0x0037, 0xf7 },
        { 0x0038, 0xf8 },
        { 0x0039, 0xf9 },
        { 0x0040, 0x7c },
        { 0x0041, 0xc1 },
        { 0x0042, 0xc2 },
        { 0x0043, 0xc3 },
        { 0x0044, 0xc4 },
        { 0x0045, 0xc5 },
        { 0x0046, 0xc6 },
        { 0x0047, 0xc7 },
        { 0x0048, 0xc8 },
        { 0x0049, 0xc9 },
        { 0x004a, 0xd1 },
        { 0x004b, 0xd2 },
        { 0x004c, 0xd3 },
        { 0x004d, 0xd4 },
        { 0x004e, 0xd5 },
        { 0x004f, 0xd6 },
        { 0x0050, 0xd7 },
        { 0x0051, 0xd8 },
        { 0x0052, 0xd9 },
        { 0x0052, 0xd9 },
        { 0x0053, 0xe2 },
        { 0x0054, 0xe3 },
        { 0x0055, 0xe4 },
        { 0x0056, 0xe5 },
        { 0x0057, 0xe6 },
        { 0x0058, 0xe7 },
        { 0x0059, 0xe8 },
        { 0x005a, 0xe9 },
        { 0x005f, 0x6d },
        { 0x00a3, 0x7b },
        { 0x00a5, 0x5b },
        { 0x00a7, 0x7c },
        { 0x00c4, 0x7b },
        { 0x00c5, 0x5b },
        { 0x00c6, 0x7b },
        { 0x00d0, 0x7c },
        { 0x00d1, 0x7b },
        { 0x00d6, 0x7c },
        { 0x00d8, 0x7c },
        { 0x00e0, 0x7c },
        { 0x0130, 0x5b },
        { 0x015e, 0x7c }
    };
}

namespace smile
{

namespace signon_text
{

UnicodeString from_signon_ebcdic(const std::vector<std::uint8_t>& ebcdic)
{
    UnicodeString result;
    for (auto ch : ebcdic)
    {
        auto found = ebcdic_to_uchar.find(ch);
        if (found == ebcdic_to_uchar.end())
            throw std::runtime_error("Invalid signon character");
        result += found->second;
    }
    return result;
}

std::vector<std::uint8_t> to_signon_ebcdic(const UnicodeString& uni)
{
    std::vector<uint8_t> result;
    for (auto i = 0; i < uni.length(); i++)
    {
        auto found = uchar_to_ebcdic.find(uni.charAt(i));
        if (found == uchar_to_ebcdic.end())
            throw std::runtime_error("Invalid signon character");
        result.push_back(found->second);
    }
    return result;
}

std::vector<std::uint8_t> to_signon_utf16(const UnicodeString& uni)
{
    std::vector<uint8_t> result;
    std::vector<uint8_t> ebcdic = to_signon_ebcdic(uni);
    for(auto ch : ebcdic)
    {
        auto found = ebcdic_to_uchar.find(ch);
        if (found == ebcdic_to_uchar.end())
            throw std::runtime_error("Invalid signon character");
        result.push_back(static_cast<uint8_t>((found->second >> 8) & 0xff));
        result.push_back(static_cast<uint8_t>(found->second & 0xff));
    }
    return result;
}

}

}