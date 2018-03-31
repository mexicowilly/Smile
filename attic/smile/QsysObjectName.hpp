#if !defined(SMILE_QSYSOBJECTNAME_HPP__)
#define SMILE_QSYSOBJECTNAME_HPP__

#include <smile/IfsObjectName.hpp>

namespace smile
{

class QsysObjectName
{
public:
    QsysObjectName();
    QsysObjectName(const Text& library,
                   const Text& object);
    QsysObjectName(const Text& library,
                   const Text& object,
                   const Text& type);
    QsysObjectName(const Text& library,
                   const Text& object,
                   const Text& type,
                   const Text& member);
    QsysObjectName(const Text& aspName,
                   const Text& library,
                   const Text& object,
                   const Text& type,
                   const Text& member);
    QsysObjectName(const QsysObjectName& name);

    QsysObjectName& operator= (const QsysObjectName& name);
    bool operator== (const QsysObjectName& name);
    bool operator!= (const QsysObjectName& name);
    bool operator< (const QsysObjectName& name);
    bool operator> (const QsysObjectName& name);

    int compare(const QsysObjectName& name);
    const Text& getAspName() const;
    const Text& getLibrary() const;
    const Text& getMember() const;
    const Text& getObject() const;
    const Text& getType() const;
    bool isEmpty() const;
    Text toApiName() const;
    Text toDisplayName() const;
    IfsObjectName toIfsName() const;

private:
    Text m_library;
    Text m_object;
    Text m_member;
    Text m_type;
    Text m_aspName;
};

inline bool QsysObjectName::operator== (const QsysObjectName& name)
{
    return compare(name) == 0;
}

inline bool QsysObjectName::operator!= (const QsysObjectName& name)
{
    return !operator==(name);
}

inline bool QsysObjectName::operator< (const QsysObjectName& name)
{
    return compare(name) < 0;
}

inline bool QsysObjectName::operator> (const QsysObjectName& name)
{
    return compare(name) > 0;
}

inline int QsysObjectName::compare(const QsysObjectName& name)
{
    return toIfsName().compare(name.toIfsName());
}

inline const Text& QsysObjectName::getAspName() const
{
    return m_aspName;
}

inline const Text& QsysObjectName::getLibrary() const
{
    return m_library;
}

inline const Text& QsysObjectName::getMember() const
{
    return m_member;
}

inline const Text& QsysObjectName::getObject() const
{
    return m_object;
}

inline const Text& QsysObjectName::getType() const
{
    return m_type;
}

inline bool QsysObjectName::isEmpty() const
{
    return m_library.isEmpty() || m_object.isEmpty();
}

}

std::ostream& operator<< (std::ostream& stream, const smile::QsysObjectName& oname);

#endif
