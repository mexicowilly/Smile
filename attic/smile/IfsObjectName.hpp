#if !defined(SMILE_IFSOBJECTNAME_HPP__)
#define SMILE_IFSOBJECTNAME_HPP__

#include <smile/Text.hpp>

namespace smile
{

class IfsObjectName
{
public:
    IfsObjectName();
    IfsObjectName(const Text& path);
    IfsObjectName(const Text& parent, const Text& object);
    IfsObjectName(const IfsObjectName& name);
    IfsObjectName(const IfsObjectName& parent, const Text& object);

    IfsObjectName& operator= (const IfsObjectName& name);
    bool operator== (const IfsObjectName& name) const;
    bool operator!= (const IfsObjectName& name) const;
    bool operator< (const IfsObjectName& name) const;
    bool operator> (const IfsObjectName& name) const;

    int compare(const IfsObjectName& name) const;
    const Text& getAbsolutePath() const;
    Text getObjectName() const;
    IfsObjectName getParentName() const;
    bool isValid() const;

private:
    Text m_path;
};

inline bool IfsObjectName::operator== (const IfsObjectName& name) const
{
    return m_path == name.m_path;
}

inline bool IfsObjectName::operator!= (const IfsObjectName& name) const
{
    return m_path != name.m_path;
}

inline bool IfsObjectName::operator< (const IfsObjectName& name) const
{
    return m_path < name.m_path;
}

inline bool IfsObjectName::operator> (const IfsObjectName& name) const
{
    return m_path > name.m_path;
}

inline int IfsObjectName::compare(const IfsObjectName& name) const
{
    return m_path.compare(name.m_path);
}

inline const Text& IfsObjectName::getAbsolutePath() const
{
    return m_path;
}

inline bool IfsObjectName::isValid() const
{
    return m_path.getLength() > 0;
}

}

std::ostream& operator<< (std::ostream& stream, const smile::IfsObjectName& ifsname);

#endif
