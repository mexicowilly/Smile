#include "TextConverterTest.hpp"
#include <smile/TextConverter.hpp>

void TextConverterTest::testEbcdicToUtf8()
{
    uint8_t ebcdic[] =
        { 0x94, 0x81, 0x49, 0x81, 0x95, 0x81, 0x40, 0xa3, 0x85, 0x40, 0x88, 0x81, 0x82, 0x93, 0x96, 0x00 };
    uint8_t utf8[] =
        { 0x6d, 0x61, 0xc3, 0xb1, 0x61, 0x6e, 0x61, 0x20, 0x74, 0x65, 0x20, 0x68, 0x61, 0x62, 0x6c, 0x6f, 0x00 };
    smile::TextConverter converter(37, "UTF-8");
    std::vector<uint8_t> converted = converter.convert(ebcdic);
    predicate(HERE, converted.size() == (sizeof(utf8) - 1));
    predicate(HERE, memcmp(utf8, &converted[0], (sizeof(utf8)- 1)) == 0);
    smile::TextConverter converter2("UTF-8", 37);
    converted = converter2.convert(utf8);
    predicate(HERE, converted.size() == (sizeof(ebcdic) - 1));
    predicate(HERE, memcmp(ebcdic, &converted[0], (sizeof(ebcdic)- 1)) == 0);
}
