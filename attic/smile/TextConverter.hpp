#if !defined(SMILE_TEXTCONVERTER_HPP__)
#define SMILE_TEXTCONVERTER_HPP__

#include <inttypes.h>
#include <pthread.h>
#include <vector>
#include <unicode/ucnv.h>

#define SMILE_DEFAULT_CONVERTER TextConverterId()

namespace smile
{

class TextConverterId
{
public:
    static const int INVALID_CCSID = -1;

    TextConverterId();
    TextConverterId(int ccsid);
    TextConverterId(const char* const name);
    TextConverterId(const TextConverterId& cid);
    ~TextConverterId();

    TextConverterId& operator= (const TextConverterId& cid);

    int getCCSID() const;
    const char* getName() const;

private:
    int m_ccsid;
    char* m_name;
};

class TextConverter
{
public:
    static int getDefaultCCSID();

    TextConverter(const TextConverterId& source, const TextConverterId& target);
    ~TextConverter();

    std::vector<uint8_t> convert(const uint8_t* const src);

private:
    TextConverter(const TextConverter& converter);
    TextConverter& operator= (const TextConverter& converter);

    UConverter* m_source;
    UConverter* m_target;
    pthread_mutex_t m_mutex;
    UChar* m_pivotBuffer;
    size_t m_pivotBufferSize;
};

inline int TextConverterId::getCCSID() const
{
    return m_ccsid;
}

inline const char* TextConverterId::getName() const
{
    return m_name;
}

}

#endif
