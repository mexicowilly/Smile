#include <smile/TextConverter.hpp>
#include <smile/Exception.hpp>
#include "LogUtil.hpp"
#include "Locker.hpp"
#include <assert.h>

namespace
{

char* copyMaybeNullStr(const char* const str)
{
    char* cpy = 0;
    if (str != 0)
    {
        size_t len = strlen(str);
        cpy = new char[len + 1];
        strcpy(cpy, str);
    }
    return cpy;
}

}

namespace smile
{

TextConverterId::TextConverterId()
    : m_ccsid(INVALID_CCSID), m_name(0)
{
}

TextConverterId::TextConverterId(int ccsid)
    : m_ccsid(ccsid), m_name(0)
{
}

TextConverterId::TextConverterId(const char* const name)
    : m_ccsid(INVALID_CCSID), m_name(copyMaybeNullStr(name))
{
}

TextConverterId::TextConverterId(const TextConverterId& cid)
    : m_ccsid(cid.m_ccsid), m_name(copyMaybeNullStr(cid.m_name))
{
}

TextConverterId::~TextConverterId()
{
    delete [] m_name;
}

TextConverterId& TextConverterId::operator= (const TextConverterId& cid)
{
    m_ccsid = cid.m_ccsid;
    delete [] m_name;
    m_name = copyMaybeNullStr(cid.m_name);
    return *this;
}

TextConverter::TextConverter(const TextConverterId& source, const TextConverterId& target)
    : m_source(0), m_target(0)
{
    UErrorCode uerr = U_ZERO_ERROR;
    if (source.getCCSID() == TextConverterId::INVALID_CCSID)
        m_source = ucnv_open(source.getName(), &uerr);
    else
        m_source = ucnv_openCCSID(source.getCCSID(), UCNV_IBM, &uerr);
    if (U_FAILURE(uerr))
    {
        throw IcuException(__FILE__, __LINE__,
                           "Could not open source converter", uerr);
    }
    uerr = U_ZERO_ERROR;
    if (target.getCCSID() == TextConverterId::INVALID_CCSID)
        m_target = ucnv_open(target.getName(), &uerr);
    else
        m_target = ucnv_openCCSID(target.getCCSID(), UCNV_IBM, &uerr);
    if (U_FAILURE(uerr))
    {
        throw IcuException(__FILE__, __LINE__,
                           "Could not open target converter", uerr);
    }
    pthread_mutex_init(&m_mutex, 0);
    m_pivotBufferSize = 8192;
    m_pivotBuffer = new UChar[m_pivotBufferSize];
}

TextConverter::~TextConverter()
{
    ucnv_close(m_source);
    ucnv_close(m_target);
    pthread_mutex_destroy(&m_mutex);
    delete [] m_pivotBuffer;
}

std::vector<uint8_t> TextConverter::convert(const uint8_t* const src)
{
    UErrorCode uerr = U_ZERO_ERROR;
    Logger& log = LogUtil::smileLogger();
    SMILE_LOG_DEBUG(log, "Converting from " << ucnv_getName(m_source, &uerr) <<
        " to " << ucnv_getName(m_target, &uerr));
    assert(U_SUCCESS(uerr));
    uerr = U_ZERO_ERROR;
    Locker locker(m_mutex);
convertToPivot:
    int32_t unicodeLen = ucnv_toUChars(m_source,
                                       m_pivotBuffer,
                                       m_pivotBufferSize,
                                       reinterpret_cast<const char*>(src),
                                       -1,
                                       &uerr);
    if (U_FAILURE(uerr))
    {
        if (uerr == U_BUFFER_OVERFLOW_ERROR)
        {
            log.debug("Reallocating pivot buffer for text conversion");
            delete [] m_pivotBuffer;
            m_pivotBufferSize = unicodeLen;
            m_pivotBuffer = new UChar[m_pivotBufferSize];
            uerr = U_ZERO_ERROR;
            goto convertToPivot;
        }
        else
        {
            throw IcuException(__FILE__, __LINE__,
                               "Could not convert to pivot", uerr);
        }
    }
    uerr = U_ZERO_ERROR;
    int32_t outputSize = UCNV_GET_MAX_BYTES_FOR_STRING(unicodeLen, ucnv_getMaxCharSize(m_target));
    std::vector<uint8_t> output = std::vector<uint8_t>(outputSize, 0);
    int32_t returnedLength = ucnv_fromUChars(m_target,
                                             reinterpret_cast<char*>(&output[0]),
                                             outputSize,
                                             m_pivotBuffer,
                                             unicodeLen,
                                             &uerr);
    if (U_FAILURE(uerr))
    {
        throw IcuException(__FILE__, __LINE__,
                           "Error converting final output", uerr);
    }
    output.resize(returnedLength);
    return output;
}

int TextConverter::getDefaultCCSID()
{
    static int defaultCCSID = -1;
    if (defaultCCSID == -1)
    {
        defaultCCSID = 37;
        UErrorCode uerr = U_ZERO_ERROR;
        UConverter* converter = ucnv_open(0, &uerr);
        if (U_SUCCESS(uerr))
        {
            uerr = U_ZERO_ERROR;
            int32_t rc = ucnv_getCCSID(converter, &uerr);
            if (U_SUCCESS(uerr))
                defaultCCSID = rc;
            ucnv_close(converter);
        }
    }
    return defaultCCSID;
}

}
