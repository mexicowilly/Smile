#if !defined(SMILE_NATIONALLANGUAGEVERSION_HPP__)
#define SMILE_NATIONALLANGUAGEVERSION_HPP__

#include <string>
#include <map>

namespace smile
{

class NationalLanguageVersion
{
public:
    static NationalLanguageVersion& getInstance();

    int getDefaultCode() const;

protected:
    friend class StaticInitializer;

    NationalLanguageVersion();

private:
    NationalLanguageVersion(const NationalLanguageVersion&);
    NationalLanguageVersion& operator= (const NationalLanguageVersion&);

    std::map<std::string, int> m_codes;
};

}

#endif
