#ifndef SMILE_TEXT_CONVERSION_HPP
#define SMILE_TEXT_CONVERSION_HPP

#include <unicode/unistr.h>

#include <string>

namespace smile
{

namespace text_conversion
{

UnicodeString utf8_to_utf16(const std::string& str);
std::string utf16_to_utf8(const UnicodeString& str);

}

}

#endif
