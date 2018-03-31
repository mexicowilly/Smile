#include <smile/Properties.hpp>
#include <smile/Exception.hpp>
#include <smile/Notifier.hpp>
#include <sstream>
#include <unicode/ustdio.h>
#include <unicode/uregex.h>
#include <unicode/uchar.h>
#include <assert.h>

namespace
{

const smile::Text FALSE_VALUE(SMILE_WSTR("FALSE"));
const smile::Text TRUE_VALUE(SMILE_WSTR("TRUE"));
const smile::Text YES_VALUE(SMILE_WSTR("YES"));

}

namespace smile
{

Text Properties::CHANGE_NOTIFICATION(SMILE_WSTR("PropertyChangeNotification"));
Text Properties::PROPERTY_NAME(SMILE_WSTR("PropertyName"));
Text Properties::OLD_VALUE(SMILE_WSTR("PropertyOldValue"));
Text Properties::NEW_VALUE(SMILE_WSTR("PropertyNewValue"));

Properties::Properties()
    : m_notify(true)
{
}

Properties::Properties(const char* const fileName, const char* const codepage)
{
    UErrorCode uerr = U_ZERO_ERROR;
    URegularExpression* commentRegex = uregex_openC("^\\s*#.*$", 0, 0, &uerr);
    if (!U_SUCCESS(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not open regular expression", uerr);
    uerr = U_ZERO_ERROR;
    URegularExpression* contentRegex = uregex_openC("^\\s*(\\S+)\\s*=\\s*(\\S+.*\\S+)??\\s*$", 0, 0, &uerr);
    if (!U_SUCCESS(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not open regular expression", uerr);
    uerr = U_ZERO_ERROR;
    URegularExpression* valueOnlyRegex = uregex_openC("^\\s*(\\S+.*\\S+)??\\s*$", 0, 0, &uerr);
    if (!U_SUCCESS(uerr))
        throw IcuException(__FILE__, __LINE__, "Could not open regular expression", uerr);
    UFILE* file = u_fopen(fileName, "r", 0, codepage);
    std::vector<UChar> chars(16 * 1024);
    std::vector<UChar> buf;
    bool continuation = false;
    Text key;
    Text value;
    unsigned line = 0;
    int32_t groupCount;
    try
    {
        while (u_fgets(&chars[0], chars.size(), file) != 0)
        {
            line++;
            size_t len = u_strlen(&chars[0]);
            if (len > 0 && chars[len - 1] == 0x000a)
                len--;
            if (len == 0)
                continue;
            uerr = U_ZERO_ERROR;
            uregex_setText(commentRegex, &chars[0], len, &uerr);
            if (!U_SUCCESS(uerr))
                throw IcuException(__FILE__, __LINE__, "Could not set regular expression text", uerr);
            uerr = U_ZERO_ERROR;
            UBool regexMatch = uregex_matches(commentRegex, 0, &uerr);
            if (!U_SUCCESS(uerr))
                throw IcuException(__FILE__, __LINE__, "Could not attempt regular expression match", uerr);
            if (!regexMatch)
            {
                UChar32 ch;
                unsigned i = 0;
                while (true)
                {
                    if (i >= len)
                        break;
                    U16_NEXT(&chars[0], i, len, ch);
                    if (!u_isblank(ch))
                        break;
                }
                if (i >= len)
                    continue;
                if (continuation)
                {
                    uerr = U_ZERO_ERROR;
                    uregex_setText(valueOnlyRegex, &chars[0], len, &uerr);
                    if (!U_SUCCESS(uerr))
                        throw IcuException(__FILE__, __LINE__, "Could not set regular expression text", uerr);
                    uerr = U_ZERO_ERROR;
                    regexMatch = uregex_matches(valueOnlyRegex, 0, &uerr);
                    if (!U_SUCCESS(uerr))
                        throw IcuException(__FILE__, __LINE__, "Could not attempt regular expression match", uerr);
                    if (regexMatch)
                    {
                        uerr = U_ZERO_ERROR;
                        groupCount = uregex_groupCount(valueOnlyRegex, &uerr);
                        if (!U_SUCCESS(uerr))
                            throw IcuException(__FILE__, __LINE__, "Could not retrieve regular expression group count", uerr);
                        if (groupCount == 1)
                        {
                            if (len > buf.size())
                                buf.resize(len);
                            uerr = U_ZERO_ERROR;
                            uregex_group(valueOnlyRegex, 1, &buf[0], buf.size(), &uerr);
                            if (!U_SUCCESS(uerr))
                                throw IcuException(__FILE__, __LINE__, "Could not retrieve regular expression group", uerr);
                            buf.resize(u_strlen(&buf[0]));
                            value += processValue(buf, continuation);
                        }
                        else
                        {
                            assert(groupCount == 0);
                            continuation = false;
                        }
                    }
                    else
                    {
                        std::ostringstream stream;
                        stream << "Invalid properties file format at line " << line << ": " << Text(&chars[0], len);
                        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
                    }
                }
                else
                {
                    uerr = U_ZERO_ERROR;
                    uregex_setText(contentRegex, &chars[0], len, &uerr);
                    if (!U_SUCCESS(uerr))
                        throw IcuException(__FILE__, __LINE__, "Could not set regular expression text", uerr);
                    uerr = U_ZERO_ERROR;
                    regexMatch = uregex_matches(contentRegex, 0, &uerr);
                    if (!U_SUCCESS(uerr))
                        throw IcuException(__FILE__, __LINE__, "Could not attempt regular expression match", uerr);
                    if (regexMatch)
                    {
                        if (len > buf.size())
                            buf.resize(len);
                        uerr = U_ZERO_ERROR;
                        uregex_group(contentRegex, 1, &buf[0], buf.size(), &uerr);
                        if (!U_SUCCESS(uerr))
                            throw IcuException(__FILE__, __LINE__, "Could not retrieve regular expression group", uerr);
                        key = Text(&buf[0], u_strlen(&buf[0]));
                        uerr = U_ZERO_ERROR;
                        groupCount = uregex_groupCount(contentRegex, &uerr);
                        if (!U_SUCCESS(uerr))
                            throw IcuException(__FILE__, __LINE__, "Could not retrieve regular expression group count", uerr);
                        if (groupCount == 2)
                        {
                            uerr = U_ZERO_ERROR;
                            uregex_group(contentRegex, 2, &buf[0], buf.size(), &uerr);
                            if (!U_SUCCESS(uerr))
                                throw IcuException(__FILE__, __LINE__, "Could not retrieve regular expression group", uerr);
                            buf.resize(u_strlen(&buf[0]));
                            value = processValue(buf, continuation);
                        }
                        else
                        {
                            assert(groupCount == 1);
                            value = Text();
                        }
                    }
                    else
                    {
                        std::ostringstream stream;
                        stream << "Invalid properties file format at line " << line << ": " << Text(&chars[0], len);
                        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
                    }
                }
                if (!continuation)
                    set(key, value);
            }
        }
    }
    catch (...)
    {
        u_fclose(file);
        uregex_close(valueOnlyRegex);
        uregex_close(contentRegex);
        uregex_close(commentRegex);
        throw;
    }
    u_fclose(file);
    uregex_close(valueOnlyRegex);
    uregex_close(contentRegex);
    uregex_close(commentRegex);
}

Properties::Properties(const Properties& props)
    : m_properties(props.m_properties)
{
}

Properties& Properties::operator= (const Properties& props)
{
    if (&props != this)
        m_properties = props.m_properties;
    return *this;
}

bool Properties::operator== (const Properties& props) const
{
    return size() == props.size() &&
           std::equal(m_properties.begin(), m_properties.end(), props.m_properties.begin());

}

const Text& Properties::get(const Text& property) const
{
    std::map<Text, Text>::const_iterator found = m_properties.find(property);
    if (found == m_properties.end())
    {
        std::ostringstream stream;
        stream << "Property not found: " << property;
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
    return found->second;
}

std::vector<std::pair<Text, Text> > Properties::getAll() const
{
    std::vector<std::pair<Text, Text> > result;
    for (std::map<Text, Text>::const_iterator itor = m_properties.begin();
         itor != m_properties.end();
         itor++)
    {
        result.push_back(std::make_pair(itor->first, itor->second));
    }
    return result;
}

bool Properties::getBool(const Text& property) const
{
    Text value(get(property));
    value.toUppercase();
    return value == TRUE_VALUE || value == YES_VALUE;
}

int64_t Properties::getInt(const Text& property) const
{
    std::istringstream stream(get(property).convertToDefault());
    int64_t value;
    stream >> value;
    return value;
}

Text Properties::processValue(std::vector<UChar>& value, bool& continuation)
{
    continuation = false;
    std::vector<UChar> result;
    bool inDoubleQuote = false;
    bool inSingleQuote = false;
    for (std::vector<UChar>::iterator itor = value.begin();
         itor != value.end();
         itor++)
    {
        // '\'
        if (*itor == 0x005c)
        {
            if (++itor == value.end())
            {
                continuation = true;
                break;
            }
            UChar replacement;
            // 't'
            if (*itor == 0x0074)
                replacement = 0x0009;
            // 'n'
            else if (*itor == 0x006e)
                replacement = 0x000a;
            // 'f'
            else if (*itor == 0x0066)
                replacement = 0x000c;
            // 'r'
            else if (*itor == 0x0072)
                replacement = 0x000d;
            else
                replacement = *itor;
            result.push_back(replacement);
        }
        else if (*itor == 0x0022)
        {
            inDoubleQuote = !inDoubleQuote;
        }
        else if (*itor == 0x0027)
        {
            inSingleQuote = !inSingleQuote;
        }
        else
        {
            result.push_back(*itor);
        }
    }
    if (inDoubleQuote)
        throw InvalidStateException(__FILE__, __LINE__, "Unmatched quotation mark in properties file");
    if (inSingleQuote)
        throw InvalidStateException(__FILE__, __LINE__, "Unmatched single quotation mark in properties file");
    return Text(&result[0], result.size());
}

void Properties::set(const Text& property, const Text& value)
{
    Properties* notificationProperties = 0;
    std::map<Text, Text>::iterator found = m_properties.find(property);
    if (found == m_properties.end() || found->second != value)
    {
        if (m_notify)
        {
            notificationProperties = new Properties();
            notificationProperties->m_notify = false;
            notificationProperties->set(PROPERTY_NAME, property);
            notificationProperties->set(NEW_VALUE, value);
            if (found != m_properties.end())
                notificationProperties->set(OLD_VALUE, found->second);
        }
        if (found == m_properties.end())
            m_properties[property] = value;
        else
            found->second = value;
        if (m_notify)
        {
            Notifier::getDefault().post(Notification(CHANGE_NOTIFICATION,
                                                     this,
                                                     *notificationProperties));
            delete notificationProperties;
        }
    }
}

void Properties::setBool(const Text& property, bool value)
{
    set(property, value ? TRUE_VALUE : FALSE_VALUE);
}

void Properties::setInt(const Text& property, int64_t value)
{
    std::ostringstream stream;
    stream << value;
    set(property, Text(stream.str().c_str()));
}

}
