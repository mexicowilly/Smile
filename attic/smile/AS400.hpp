#if !defined(SMILE_AS400_HPP__)
#define SMILE_AS400_HPP__

#include <smile/Text.hpp>
#include <smile/OS400Version.hpp>

namespace smile
{

class AS400Impl;

class AS400
{
public:
    AS400(const Text& systemName, const Text& userName, const Text& password);
    virtual ~AS400();

    int getCCSID();
    AS400Impl& getImplementation();
    const Text& getSystemName() const;
    const Text& getUserName() const;
    const OS400Version& getVersion();

private:
    AS400(const AS400& as400);
    AS400& operator= (const AS400& as400);

    AS400Impl* m_impl;
};

inline AS400Impl& AS400::getImplementation()
{
    return *m_impl;
}

}

#endif
