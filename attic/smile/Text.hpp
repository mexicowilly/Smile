#if !defined(SMILE_TEXT_HPP__)
#define SMILE_TEXT_HPP__

#include <unicode/ustring.h>
#include <unicode/udat.h>
#include <smile/TextConverter.hpp>
#include <smile/Time.hpp>
#include <smile/TextMacros.hpp>
#include <ostream>
#include <limits>

namespace smile
{

class Text
{
public:
    static const unsigned npos;

    enum TimeFormat
    {
        TimeFormat_Short     = UDAT_SHORT,
        TimeFormat_Medium    = UDAT_MEDIUM,
        TimeFormat_Long      = UDAT_LONG,
        TimeFormat_Full      = UDAT_FULL
    };

    Text();
    Text(const UChar* const chars, size_t length = std::numeric_limits<size_t>::max());
    Text(const uint8_t* const chars,
         int length,
         const TextConverterId& converter);
    Text(const std::vector<uint8_t>& chars,
         const TextConverterId& converter);
    Text(const char* const chars,
         const TextConverterId& converter = SMILE_DEFAULT_CONVERTER);
    explicit Text(Time time, TimeFormat format = TimeFormat_Medium);
#if U_SIZEOF_WCHAR_T != 2
    Text(const wchar_t* const wchars, size_t length = std::numeric_limits<size_t>::max());
#endif
    Text(const Text& text);
    ~Text();

    Text& operator= (const Text& text);
    bool operator== (const Text& text) const;
    bool operator!= (const Text& text) const;
    bool operator< (const Text& text) const;
    bool operator> (const Text& text) const;
    Text& operator+= (const Text& text);
    UChar operator[] (unsigned offset) const;

    int compare(const Text& text) const;
    int compareNoCase(const Text& text) const;
    std::vector<uint8_t> convert(const TextConverterId& converter) const;
    std::string convertToDefault() const;
    unsigned findFirst(UChar ch) const;
    unsigned findLast(UChar ch) const;
    UChar getAt(unsigned offset) const;
    const UChar* getUChars() const;
    size_t getLength() const;
    bool isEmpty() const;
    bool isOnlyDigits() const;
    Text subText(unsigned start, size_t length = std::numeric_limits<size_t>::max()) const;
    std::vector<Text> tokenize(const Text& delimiter) const;
    Text& toUppercase();
    Text& trim();

private:
    UChar* m_chars;
    size_t m_length;
};

inline bool Text::operator== (const Text& text) const
{
    return compare(text) == 0;
}

inline bool Text::operator!= (const Text& text) const
{
    return !operator==(text);
}

inline bool Text::operator< (const Text& text) const
{
    return compare(text) < 0;
}

inline bool Text::operator> (const Text& text) const
{
    return compare(text) > 0;
}

inline UChar Text::operator[] (unsigned offset) const
{
    return getAt(offset);
}

inline const UChar* Text::getUChars() const
{
    return m_chars;
}

inline size_t Text::getLength() const
{
    return m_length;
}

inline bool Text::isEmpty() const
{
    return m_length == 0;
}

}

smile::Text operator+ (const smile::Text& left, const smile::Text& right);
std::ostream& operator<< (std::ostream& stream, const smile::Text& text);

#endif
