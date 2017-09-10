#ifndef SMILE_SIGNON_TEXT_HPP
#define SMILE_SIGNON_TEXT_HPP

#include <unicode/unistr.h>
#include <vector>

namespace smile
{

namespace signon_text
{

UnicodeString from_signon_ebcdic(const std::vector<std::uint8_t>& ebcdic);
std::vector<std::uint8_t> to_signon_ebcdic(const UnicodeString& uni);
std::vector<std::uint8_t> to_signon_utf16(const UnicodeString& uni);

}

}

#endif
