#include "NationalLanguageVersion.hpp"
#include "StaticInitializer.hpp"
#include <unicode/uloc.h>
#include <assert.h>

namespace smile
{

NationalLanguageVersion& NationalLanguageVersion::getInstance()
{
    return StaticInitializer::getInstance().getNationalLanguageVersion();
}

NationalLanguageVersion::NationalLanguageVersion()
{
    m_codes["sq"]       = 2995; // Albanian
    m_codes["ar"]       = 2954; // Arabic
    m_codes["nl_BE"]    = 2963; // Belgian Dutch
    m_codes["en_BE"]    = 2909; // Belgian English
    m_codes["fr_BE"]    = 2966; // Belgian French
    m_codes["pt_BR"]    = 2980; // Brazilian Portuguese
    m_codes["bg"]       = 2974; // Bulgarian
    m_codes["fr_CA"]    = 2981; // Canadian French
    m_codes["hr"]       = 2912; // Croatian
    m_codes["cs"]       = 2975; // Czech
    m_codes["da"]       = 2926; // Danish
    m_codes["nl"]       = 2923; // Dutch Netherlands
    m_codes["en"]       = 2924; // English
    m_codes["en_JP"]    = 2938; // English uppercase support for DBCS
    m_codes["en_CN"]    = 2984; // English uppercase and lowercase support for DBCS
    m_codes["en_KR"]    = 2984;
    m_codes["en_SG"]    = 2984;
    m_codes["en_TW"]    = 2984;
    m_codes["et"]       = 2902; // Estonian
    m_codes["fa"]       = 2998; // Farsi
    m_codes["fi"]       = 2925; // Finnish
    m_codes["fr"]       = 2928; // French
    m_codes["fr_CH"]    = 2940; // French multinational character set
    m_codes["de"]       = 2929; // German
    m_codes["de_CH"]    = 2939; // German multinational character set
    m_codes["el"]       = 2957; // Greek
    m_codes["he"]       = 2961; // Hebrew
    m_codes["hu"]       = 2976; // Hungarian
    m_codes["is"]       = 2958; // Icelandic
    m_codes["it"]       = 2932; // Italian
    m_codes["it_CH"]    = 2942; // Italian multinational character set
    // Missing Japanese Universal 2930
    m_codes["ja"]       = 2962; // Japanese Kanji
    m_codes["ko"]       = 2986; // Korean
    m_codes["lo"]       = 2906; // Lao
    m_codes["lv"]       = 2904; // Latvian
    m_codes["lt"]       = 2903; // Lithuanian
    m_codes["mk"]       = 2913; // Macedonian
    m_codes["no"]       = 2933; // Norwegian
    m_codes["pl"]       = 2978; // Polish
    m_codes["pt_PT"]    = 2922; // Portuguese
    m_codes["pt"]       = 2996; // Portuguese multinational character set
    m_codes["ro"]       = 2992; // Romanian
    m_codes["ru"]       = 2979; // Russian
    m_codes["sr"]       = 2914; // Serbian
    m_codes["zh_CN"]    = 2989; // Simplified Chinese
    m_codes["zh_SG"]    = 2989;
    m_codes["zh"]       = 2989;
    m_codes["sk"]       = 2994; // Slovakian
    m_codes["sl"]       = 2911; // Slovenian
    m_codes["es"]       = 2931; // Spanish
    m_codes["sv"]       = 2937; // Swedish
    m_codes["th"]       = 2972; // Thai
    m_codes["zh_HK"]    = 2987; // Traditional Chinese
    m_codes["zh_TW"]    = 2987;
    m_codes["tr"]       = 2956; // Turkish
    m_codes["vi"]       = 2905; // Vietnamese
}

int NationalLanguageVersion::getDefaultCode() const
{
    char buf[128];
    UErrorCode uerr = U_ZERO_ERROR;
    uloc_getName(uloc_getDefault(), buf, 128, &uerr);
    assert(U_SUCCESS(uerr));
    std::string name(U_SUCCESS(uerr) ? buf : "en");
    std::map<std::string, int>::const_iterator itor = m_codes.find(name);
    if (itor == m_codes.end())
    {
        name = name.substr(0, name.find('_'));
        itor = m_codes.find(name);
        if (itor == m_codes.end())
            itor = m_codes.find("en");
        assert(itor != m_codes.end());
    }
    return itor->second;
}

}
