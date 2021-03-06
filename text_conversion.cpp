#include "text_conversion.hpp"
#include "icu_exception.hpp"
#include "garbage_cleaner.hpp"

#include <unicode/ucnv.h>
#include <boost/thread/shared_mutex.hpp>

#include <map>
#include <memory>
#include <vector>
#include <mutex>

namespace
{

struct converter
{
    converter(std::uint16_t ccsid);
    ~converter();

    UConverter* cnv;
};

converter::converter(std::uint16_t ccsid)
{
    UErrorCode uerr = U_ZERO_ERROR;
    cnv = ucnv_openCCSID(ccsid, UCNV_IBM, &uerr);
    if (U_FAILURE(uerr))
        throw smile::icu_exception("Error instantiating a codepage converter", uerr);
}

converter::~converter()
{
    ucnv_close(cnv);
}

struct static_data
{
    static_data()
    {
        smile::garbage_cleaner::get().add([this] () { delete this; });
    }

    std::map<std::uint16_t, std::unique_ptr<converter>> converters;
    boost::shared_mutex guard;
};

static_data& statics()
{
    static std::once_flag once;
    static static_data* sd;

    std::call_once(once, [&] () { sd = new static_data(); });
    return *sd;
}

converter& get_converter(std::uint16_t ccsid)
{
    converter* result;
    auto& sd = statics();
    sd.guard.lock_shared();
    auto found = sd.converters.find(ccsid);
    if (found == sd.converters.end())
    {
        sd.guard.unlock();
        sd.guard.lock();
        auto p = sd.converters.emplace(ccsid, std::make_unique<converter>(ccsid));
        result = p.first->second.get();
    }
    else
    {
        result = found->second.get();
    }
    sd.guard.unlock();
    return *result;
}

}

namespace smile
{

namespace text_conversion
{

UnicodeString utf8_to_utf16(const std::string& str)
{
    converter& c(get_converter(1208));
    std::vector<UChar> buf(str.length() + 1);
    UErrorCode uerr;
    std::int32_t ulen;
    do
    {
        uerr = U_ZERO_ERROR;
        ulen = ucnv_toUChars(c.cnv,
                             &buf[0],
                             buf.size(),
                             str.c_str(),
                             str.length(),
                             &uerr);
        if (U_FAILURE(uerr))
        {
            if (uerr == U_BUFFER_OVERFLOW_ERROR)
                buf.resize(ulen);
            else
                throw icu_exception("Error converting from UTF-8 to UTF-16", uerr);
        }
    } while (uerr == U_BUFFER_OVERFLOW_ERROR);
    return UnicodeString(&buf[0], ulen);
}

std::string utf16_to_utf8(const UnicodeString& str)
{
    converter& c(get_converter(1208));
    std::int32_t out_size = UCNV_GET_MAX_BYTES_FOR_STRING(str.length(), ucnv_getMaxCharSize(c.cnv));
    std::string result(out_size, 0);
    UErrorCode uerr;
    std::int32_t len = ucnv_fromUChars(c.cnv,
                                       &result[0],
                                       out_size,
                                       str.getBuffer(),
                                       str.length(),
                                       &uerr);
    if (U_FAILURE(uerr))
        throw icu_exception("Error converting from UTF-16 to UTF-8", uerr);
    result.resize(len);
    return result;
}

}

}