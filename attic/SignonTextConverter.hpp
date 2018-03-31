#if !defined(SMILE_SIGNONTEXTCONVERTER_HPP__)
#define SMILE_SIGNONTEXTCONVERTER_HPP__

#include <smile/Text.hpp>
#include <map>

namespace smile
{

class SignonTextConverter
{
public:
    static const SignonTextConverter& getInstance();

    Text convertFromSignonEbcdic(const std::vector<uint8_t>& bytes) const;
    std::vector<uint8_t> convertToSignonEbcdic(const Text& text) const;
    std::vector<uint8_t> convertToSignonUtf16(const Text& text) const;

protected:
    friend class StaticInitializer;

    SignonTextConverter();

private:
    SignonTextConverter(const SignonTextConverter& converter);
    SignonTextConverter& operator= (const SignonTextConverter& converter);

    std::map<uint8_t, UChar> m_byteToUChar;
    std::map<UChar, uint8_t> m_uCharToByte;
};

}

#endif
