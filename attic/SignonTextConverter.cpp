#include "SignonTextConverter.hpp"
#include "StaticInitializer.hpp"
#include <smile/Exception.hpp>

namespace smile
{

SignonTextConverter::SignonTextConverter()
{
    m_byteToUChar[0x40] = 0x0020;
    m_byteToUChar[0x5b] = 0x0024;
    m_byteToUChar[0x6d] = 0x005f;
    m_byteToUChar[0x7b] = 0x0023;
    m_byteToUChar[0x7c] = 0x0040;
    m_byteToUChar[0xc1] = 0x0041;
    m_byteToUChar[0xc2] = 0x0042;
    m_byteToUChar[0xc3] = 0x0043;
    m_byteToUChar[0xc4] = 0x0044;
    m_byteToUChar[0xc5] = 0x0045;
    m_byteToUChar[0xc6] = 0x0046;
    m_byteToUChar[0xc7] = 0x0047;
    m_byteToUChar[0xc8] = 0x0048;
    m_byteToUChar[0xc9] = 0x0049;
    m_byteToUChar[0xd1] = 0x004a;
    m_byteToUChar[0xd2] = 0x004b;
    m_byteToUChar[0xd3] = 0x004c;
    m_byteToUChar[0xd4] = 0x004d;
    m_byteToUChar[0xd5] = 0x004e;
    m_byteToUChar[0xd6] = 0x004f;
    m_byteToUChar[0xd7] = 0x0050;
    m_byteToUChar[0xd8] = 0x0051;
    m_byteToUChar[0xd9] = 0x0052;
    m_byteToUChar[0xe2] = 0x0053;
    m_byteToUChar[0xe3] = 0x0054;
    m_byteToUChar[0xe4] = 0x0055;
    m_byteToUChar[0xe5] = 0x0056;
    m_byteToUChar[0xe6] = 0x0057;
    m_byteToUChar[0xe7] = 0x0058;
    m_byteToUChar[0xe8] = 0x0059;
    m_byteToUChar[0xe9] = 0x005a;
    m_byteToUChar[0xf0] = 0x0030;
    m_byteToUChar[0xf1] = 0x0031;
    m_byteToUChar[0xf2] = 0x0032;
    m_byteToUChar[0xf3] = 0x0033;
    m_byteToUChar[0xf4] = 0x0034;
    m_byteToUChar[0xf5] = 0x0035;
    m_byteToUChar[0xf6] = 0x0036;
    m_byteToUChar[0xf7] = 0x0037;
    m_byteToUChar[0xf8] = 0x0038;
    m_byteToUChar[0xf9] = 0x0039;

    m_uCharToByte[0x0023] = 0x7b;
    m_uCharToByte[0x0024] = 0x5b;
    m_uCharToByte[0x0030] = 0xf0;
    m_uCharToByte[0x0031] = 0xf1;
    m_uCharToByte[0x0032] = 0xf2;
    m_uCharToByte[0x0033] = 0xf3;
    m_uCharToByte[0x0034] = 0xf4;
    m_uCharToByte[0x0035] = 0xf5;
    m_uCharToByte[0x0036] = 0xf6;
    m_uCharToByte[0x0037] = 0xf7;
    m_uCharToByte[0x0038] = 0xf8;
    m_uCharToByte[0x0039] = 0xf9;
    m_uCharToByte[0x0040] = 0x7c;
    m_uCharToByte[0x0041] = 0xc1;
    m_uCharToByte[0x0042] = 0xc2;
    m_uCharToByte[0x0043] = 0xc3;
    m_uCharToByte[0x0044] = 0xc4;
    m_uCharToByte[0x0045] = 0xc5;
    m_uCharToByte[0x0046] = 0xc6;
    m_uCharToByte[0x0047] = 0xc7;
    m_uCharToByte[0x0048] = 0xc8;
    m_uCharToByte[0x0049] = 0xc9;
    m_uCharToByte[0x004a] = 0xd1;
    m_uCharToByte[0x004b] = 0xd2;
    m_uCharToByte[0x004c] = 0xd3;
    m_uCharToByte[0x004d] = 0xd4;
    m_uCharToByte[0x004e] = 0xd5;
    m_uCharToByte[0x004f] = 0xd6;
    m_uCharToByte[0x0050] = 0xd7;
    m_uCharToByte[0x0051] = 0xd8;
    m_uCharToByte[0x0052] = 0xd9;
    m_uCharToByte[0x0052] = 0xd9;
    m_uCharToByte[0x0053] = 0xe2;
    m_uCharToByte[0x0054] = 0xe3;
    m_uCharToByte[0x0055] = 0xe4;
    m_uCharToByte[0x0056] = 0xe5;
    m_uCharToByte[0x0057] = 0xe6;
    m_uCharToByte[0x0058] = 0xe7;
    m_uCharToByte[0x0059] = 0xe8;
    m_uCharToByte[0x005a] = 0xe9;
    m_uCharToByte[0x005f] = 0x6d;
    m_uCharToByte[0x00a3] = 0x7b;
    m_uCharToByte[0x00a5] = 0x5b;
    m_uCharToByte[0x00a7] = 0x7c;
    m_uCharToByte[0x00c4] = 0x7b;
    m_uCharToByte[0x00c5] = 0x5b;
    m_uCharToByte[0x00c6] = 0x7b;
    m_uCharToByte[0x00d0] = 0x7c;
    m_uCharToByte[0x00d1] = 0x7b;
    m_uCharToByte[0x00d6] = 0x7c;
    m_uCharToByte[0x00d8] = 0x7c;
    m_uCharToByte[0x00e0] = 0x7c;
    m_uCharToByte[0x0130] = 0x5b;
    m_uCharToByte[0x015e] = 0x7c;
}

Text SignonTextConverter::convertFromSignonEbcdic(const std::vector<uint8_t>& bytes) const
{
    std::vector<UChar> uchars;
    for (std::vector<uint8_t>::const_iterator itor = bytes.begin();
         itor != bytes.end();
         itor++)
    {
        std::map<uint8_t, UChar>::const_iterator found = m_byteToUChar.find(*itor);
        if (found == m_byteToUChar.end())
            throw InvalidStateException(__FILE__, __LINE__, "Invalid signon character");
        uchars.push_back(found->second);
    }
    return Text(&uchars[0], uchars.size());
}

std::vector<uint8_t> SignonTextConverter::convertToSignonEbcdic(const Text& text) const
{
    std::vector<uint8_t> output;
    for (unsigned i = 0; i < text.getLength(); i++)
    {
        std::map<UChar, uint8_t>::const_iterator found = m_uCharToByte.find(text.getAt(i));
        if (found == m_uCharToByte.end())
            throw InvalidStateException(__FILE__, __LINE__, "Invalid signon character");
        output.push_back(found->second);
    }
    return output;
}

std::vector<uint8_t> SignonTextConverter::convertToSignonUtf16(const Text& text) const
{
    std::vector<uint8_t> ebcdic = convertToSignonEbcdic(text);
    std::vector<uint8_t> utf16;
    for (std::vector<uint8_t>::const_iterator itor = ebcdic.begin();
         itor != ebcdic.end();
         itor++)
    {
        std::map<uint8_t, UChar>::const_iterator found = m_byteToUChar.find(*itor);
        if (found == m_byteToUChar.end())
            throw InvalidStateException(__FILE__, __LINE__, "Invalid signon character");
        utf16.push_back(static_cast<uint8_t>((found->second >> 8) & 0xff));
        utf16.push_back(static_cast<uint8_t>(found->second & 0xff));
    }
    return utf16;
}

const SignonTextConverter& SignonTextConverter::getInstance()
{
    return StaticInitializer::getInstance().getSignonTextConverter();
}

}
