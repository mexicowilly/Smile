#include <smile/Text.hpp>
#include <smile/Exception.hpp>
#include "LogUtil.hpp"
#include <assert.h>
#include <unicode/uchar.h>

smile::Text operator+ (const smile::Text& left, const smile::Text& right)
{
    smile::Text result(left);
    result += right;
    return result;
}

std::ostream& operator<< (std::ostream& stream, const smile::Text& text)
{
    try
    {
        stream << text.convertToDefault().c_str();
    }
    catch (...)
    {
    }
    return stream;
}

namespace smile
{

const unsigned Text::npos = std::numeric_limits<unsigned>::max();

Text::Text()
    : m_chars(0), m_length(0)
{
}

Text::Text(const UChar* const chars, size_t length)
{
    if (length == std::numeric_limits<size_t>::max())
        length = u_strlen(chars);
    m_length = length;
    if (length == 0)
    {
        m_chars = 0;
    }
    else
    {
        m_chars = new UChar[length];
        u_strncpy(m_chars, chars, length);
    }
}

Text::Text(const uint8_t* const chars,
           int length,
           const TextConverterId& converter)
    : m_length(0)
{
    if (length == 0)
    {
        m_chars = 0;
    }
    else
    {
        if (length < 0)
            length = -1;
        UErrorCode uerr = U_ZERO_ERROR;
        UConverter* uconv = converter.getCCSID() == TextConverterId::INVALID_CCSID ?
            ucnv_open(converter.getName(), &uerr) :
            ucnv_openCCSID(converter.getCCSID(), UCNV_IBM, &uerr);
        if (U_FAILURE(uerr))
        {
            throw IcuException(__FILE__, __LINE__,
                               "Could not open converter", uerr);
        }
        uerr = U_ZERO_ERROR;
        int32_t unicodeLen = ucnv_toUChars(uconv,
                                           0,
                                           0,
                                           reinterpret_cast<const char*>(chars),
                                           length,
                                           &uerr);
        if (U_FAILURE(uerr) && uerr != U_BUFFER_OVERFLOW_ERROR)
        {
            ucnv_close(uconv);
            throw IcuException(__FILE__, __LINE__,
                               "Could not calculate number of Unicode characters required", uerr);
        }
        if (unicodeLen == 0)
        {
            m_chars = 0;
        }
        else
        {
            m_chars = new UChar[unicodeLen];
            uerr = U_ZERO_ERROR;
            ucnv_toUChars(uconv,
                          m_chars,
                          unicodeLen,
                          reinterpret_cast<const char*>(chars),
                          length,
                          &uerr);
            if (U_FAILURE(uerr))
            {
                delete [] m_chars;
                m_chars = 0;
                ucnv_close(uconv);
                throw IcuException(__FILE__, __LINE__,
                                   "Could not convert to Unicode", uerr);
            }
            ucnv_close(uconv);
        }
        m_length = unicodeLen;
    }
}

Text::Text(const std::vector<uint8_t>& chars,
           const TextConverterId& converter)
{
    if (chars.size() == 0)
    {
        m_chars = 0;
        m_length = 0;
    }
    else
    {
        Text temp(&chars[0], chars.size(), converter);
        m_chars = temp.m_chars;
        m_length = temp.m_length;
        temp.m_chars = 0;
        temp.m_length = 0;
    }

}

Text::Text(const char* const chars, const TextConverterId& converter)
{
    if (chars == 0 || strlen(chars) == 0)
    {
        m_chars = 0;
        m_length = 0;
    }
    else
    {
        Text temp(reinterpret_cast<const uint8_t*>(chars), -1, converter);
        m_chars = temp.m_chars;
        m_length = temp.m_length;
        temp.m_chars = 0;
        temp.m_length = 0;
    }
}


Text::Text(Time time, TimeFormat format)
    : m_chars(0), m_length(0)
{
    UErrorCode uerr = U_ZERO_ERROR;
    // Use C-style cast here because on Solaris the Sun CC
    // compiler refuses to static_cast or reinterpret_cast
    // this type
    UDateFormat* df = udat_open((UDateFormatStyle)format,
                                (UDateFormatStyle)format,
                                0, 0, -1, 0, 0, &uerr);
    if (!U_SUCCESS(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not open date format", uerr);
    uerr = U_ZERO_ERROR;
    m_length = udat_format(df, time, 0, 0, 0, &uerr);
    if (uerr != U_BUFFER_OVERFLOW_ERROR)
    {
        m_length = 0;
        udat_close(df);
        throw IcuException(__FILE__, __LINE__, "Could not calculate buffer size", uerr);
    }
    m_chars = new UChar[m_length + 1];
    uerr = U_ZERO_ERROR;
    udat_format(df, time, m_chars, m_length + 1, 0, &uerr);
    udat_close(df);
    if (!U_SUCCESS(uerr))
    {
        delete [] m_chars;
        m_chars = 0;
        m_length = 0;
        throw IcuException(__FILE__, __LINE__, "Could not format time", uerr);
    }
}

#if U_SIZEOF_WCHAR_T != 2
Text::Text(const wchar_t* const wchars, size_t length)
{
    if (length == std::numeric_limits<size_t>::max())
        length = wcslen(wchars);
    if (length == 0)
    {
        m_chars = 0;
    }
    else
    {
        m_length = length;
        m_chars = new UChar[m_length];
        UErrorCode uerr = U_ZERO_ERROR;
        int32_t required;
        u_strFromWCS(m_chars,
                     m_length,
                     &required,
                     wchars,
                     length,
                     &uerr);
        if (!U_SUCCESS(uerr))
            throw IcuException(__FILE__, __LINE__, "Could not covert from wcs", uerr);
        if (static_cast<size_t>(required) > m_length)
        {
            SMILE_LOG_INFO(LogUtil::smileLogger(),
                "Reallocation required when converting from WCS. Original buffer was " <<
                m_length << ", but " << required << " bytes were required");
            m_length = required;
            delete [] m_chars;
            m_chars = new UChar[m_length];
            uerr = U_ZERO_ERROR;
            u_strFromWCS(m_chars,
                         m_length,
                         &required,
                         wchars,
                         length,
                         &uerr);
            if (!U_SUCCESS(uerr))
                throw IcuException(__FILE__, __LINE__, "Could not covert from wcs", uerr);
        }
    }
}
#endif

Text::Text(const Text& text)
{
    if (text.m_length > 0)
    {
        m_length = text.m_length;
        m_chars = new UChar[m_length];
        u_strncpy(m_chars, text.m_chars, m_length);
    }
    else
    {
        m_length = 0;
        m_chars = 0;
    }
}

Text::~Text()
{
    delete [] m_chars;
}

Text& Text::operator= (const Text& text)
{
    if (&text != this)
    {
        delete [] m_chars;
        if (text.m_length > 0)
        {
            m_length = text.m_length;
            m_chars = new UChar[m_length];
            u_strncpy(m_chars, text.m_chars, m_length);
        }
        else
        {
            m_length = 0;
            m_chars = 0;
        }
    }
    return *this;
}

Text& Text::operator+= (const Text& text)
{
    if (m_chars == 0)
    {
        operator=(text);
    }
    else if (text.m_chars != 0)
    {
        UChar* newChars = new UChar[m_length + text.m_length];
        u_strncpy(newChars, m_chars, m_length);
        u_strncpy(newChars + m_length, text.m_chars, text.m_length);
        delete [] m_chars;
        m_chars = newChars;
        m_length += text.m_length;
    }
    return *this;
}

int Text::compare(const Text& text) const
{
    if (m_chars == 0)
    {
        return -text.m_length;
    }
    else if (text.m_chars == 0)
    {
        return text.m_length;
    }
    assert(m_length > 0);
    assert(text.m_length > 0);
    return u_strCompare(m_chars, m_length, text.m_chars, text.m_length, FALSE);
}

int Text::compareNoCase(const Text& text) const
{
    if (m_chars == 0)
    {
        return -text.m_length;
    }
    else if (text.m_chars == 0)
    {
        return text.m_length;
    }
    assert(m_length > 0);
    assert(text.m_length > 0);
    UErrorCode uerr = U_ZERO_ERROR;
    int32_t result = u_strCaseCompare(m_chars,
                                      m_length,
                                      text.m_chars,
                                      text.m_length,
                                      U_FOLD_CASE_DEFAULT,
                                      &uerr);
    if (!U_SUCCESS(uerr))
        throw IcuException(__FILE__, __LINE__, "Error comparing text without case", uerr);
    return result;
}

std::vector<uint8_t> Text::convert(const TextConverterId& converter) const
{
    if (m_length == 0)
        return std::vector<uint8_t>();
    UErrorCode uerr = U_ZERO_ERROR;
    UConverter* uconv = converter.getCCSID() == TextConverterId::INVALID_CCSID ?
        ucnv_open(converter.getName(), &uerr) :
        ucnv_openCCSID(converter.getCCSID(), UCNV_IBM, &uerr);
    if (U_FAILURE(uerr))
    {
        throw IcuException(__FILE__, __LINE__,
                           "Could not open converter", uerr);
    }
    uerr = U_ZERO_ERROR;
    int32_t requiredLen = ucnv_fromUChars(uconv,
                                          0,
                                          0,
                                          m_chars,
                                          m_length,
                                          &uerr);
    if (U_FAILURE(uerr) && uerr != U_BUFFER_OVERFLOW_ERROR)
    {
        ucnv_close(uconv);
        throw IcuException(__FILE__, __LINE__,
                           "Could not calculate number of characters required", uerr);
    }
    std::vector<uint8_t> output(requiredLen);
    uerr = U_ZERO_ERROR;
    ucnv_fromUChars(uconv,
                    reinterpret_cast<char*>(&output[0]),
                    requiredLen,
                    m_chars,
                    m_length,
                    &uerr);
    if (U_FAILURE(uerr))
    {
        ucnv_close(uconv);
        throw IcuException(__FILE__, __LINE__,
                           "Could not convert to Unicode", uerr);
    }
    ucnv_close(uconv);
    return output;
}

std::string Text::convertToDefault() const
{
    std::vector<uint8_t> converted = convert(SMILE_DEFAULT_CONVERTER);
    return std::string(reinterpret_cast<const char*>(&converted[0]), converted.size());
}

unsigned Text::findFirst(UChar ch) const
{
    unsigned found = npos;
    if (m_length != 0)
    {
        UChar* pos = u_memchr(m_chars, ch, m_length);
        if (pos != 0)
            found = pos - m_chars;
    }
    return found;
}

unsigned Text::findLast(UChar ch) const
{
    unsigned found = npos;
    if (m_length != 0)
    {
        UChar* pos = u_memrchr(m_chars, ch, m_length);
        if (pos != 0)
            found = pos - m_chars;
    }
    return found;
}

UChar Text::getAt(unsigned offset) const
{
    assert(offset < m_length);
    return m_chars[offset];
}

bool Text::isOnlyDigits() const
{
    if (m_length == 0)
        return false;
    UChar32 ch;
    unsigned i = 0;
    while (true)
    {
        if (i >= m_length)
            break;
        U16_NEXT(m_chars, i, m_length, ch);
        if (!u_getIntPropertyValue(ch, UCHAR_NUMERIC_TYPE) != U_NT_DIGIT)
            return false;
    }
    return true;
}

Text Text::subText(unsigned start, size_t length) const
{
    if (start >= m_length)
        throw InvalidStateException(__FILE__, __LINE__, "Index out of bounds");
    if (length == std::numeric_limits<size_t>::max() ||
        (start + length) > m_length)
    {
        length = m_length - start;
    }
    return Text(m_chars + start, length);
}

std::vector<Text> Text::tokenize(const Text& delimiter) const
{
    std::vector<Text> output;
    if (m_length == 0)
        return output;
    std::vector<UChar> chars(m_chars, m_chars + m_length);
    chars.push_back(0x0000);
    std::vector<UChar> delim(delimiter.m_chars, delimiter.m_chars + delimiter.m_length);
    delim.push_back(0x0000);
    UChar* state;
    UChar* token = u_strtok_r(&chars[0], &delim[0], &state);
    while (token != 0)
    {
        output.push_back(Text(token, u_strlen(token)));
        token = u_strtok_r(0, &delim[0], &state);
    }
    return output;
}

Text& Text::toUppercase()
{
    if (!isEmpty())
    {
        UErrorCode uerr = U_ZERO_ERROR;
        size_t required = u_strToUpper(0, 0, m_chars, m_length, 0, &uerr);
        if (U_FAILURE(uerr) && uerr != U_BUFFER_OVERFLOW_ERROR)
            throw IcuException(__FILE__, __LINE__, "Could not preflight case folding", uerr);
        uerr = U_ZERO_ERROR;
        if (required == m_length)
        {
            u_strToUpper(m_chars, m_length, m_chars, m_length, 0, &uerr);
            if (U_FAILURE(uerr))
                throw IcuException(__FILE__, __LINE__, "Could not convert to upper case", uerr);
        }
        else
        {
            UChar* newChars = new UChar[required];
            u_strToUpper(newChars, required, m_chars, m_length, 0, &uerr);
            if (U_FAILURE(uerr))
                throw IcuException(__FILE__, __LINE__, "Could not convert to upper case", uerr);
            delete [] m_chars;
            m_chars = newChars;
            m_length = required;
        }
    }
    return *this;
}

Text& Text::trim()
{
    if (m_length != 0)
    {
        UChar32 ch;
        size_t i = m_length;
        size_t length;
        while (true)
        {
            length = i;
            if (i <= 0)
                break;
            U16_PREV(m_chars, 0, i, ch);
            if (!u_isWhitespace(ch))
                break;
        }
        if (length < m_length)
            m_length = length;
        size_t start;
        i = 0;
        while (true)
        {
            start = i;
            if (i >= m_length)
                break;
            U16_NEXT(m_chars, i, m_length, ch);
            if (!u_isWhitespace(ch))
                break;
        }
        if (start > 0)
        {
            m_length -= start;
            memmove(m_chars, m_chars + start, m_length * sizeof(UChar));
        }
    }
    if (m_length == 0)
    {
        delete [] m_chars;
        m_chars = 0;
    }
    return *this;
}

}
