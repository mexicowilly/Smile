#include "smile/QsysObjectName.hpp"

std::ostream& operator<< (std::ostream& stream, const smile::QsysObjectName& oname)
{
    stream << oname.toDisplayName();
    return stream;
}

namespace smile
{

QsysObjectName::QsysObjectName()
{
}

QsysObjectName::QsysObjectName(const Text& library,
                               const Text& object)
    : m_library(library),
      m_object(object)
{
    m_library.trim();
    m_object.trim();
}

QsysObjectName::QsysObjectName(const Text& library,
                               const Text& object,
                               const Text& type)
    : m_library(library),
      m_object(object),
      m_type(type)
{
    m_library.trim();
    m_object.trim();
    m_type.trim();
}

QsysObjectName::QsysObjectName(const Text& library,
                               const Text& object,
                               const Text& type,
                               const Text& member)
    : m_library(library),
      m_object(object),
      m_member(member),
      m_type(type)
{
    m_library.trim();
    m_object.trim();
    m_member.trim();
    m_type.trim();
}

QsysObjectName::QsysObjectName(const Text& aspName,
                               const Text& library,
                               const Text& object,
                               const Text& type,
                               const Text& member)
    : m_library(library),
      m_object(object),
      m_member(member),
      m_type(type),
      m_aspName(aspName)
{
    m_library.trim();
    m_object.trim();
    m_member.trim();
    m_type.trim();
    m_aspName.trim();
}

QsysObjectName::QsysObjectName(const QsysObjectName& name)
    : m_library(name.m_library),
      m_object(name.m_object),
      m_member(name.m_member),
      m_type(name.m_type),
      m_aspName(name.m_aspName)
{
}

QsysObjectName& QsysObjectName::operator= (const QsysObjectName& name)
{
    if (&name != this)
    {
        m_library = name.m_library;
        m_object = name.m_object;
        m_member = name.m_member;
        m_type = name.m_type;
        m_aspName = name.m_aspName;
    }
    return *this;
}

Text QsysObjectName::toApiName() const
{
    UChar apiName[20];
    u_memset(apiName, 0x0020, 20);
    u_strncpy(apiName, m_object.getUChars(), std::min(m_object.getLength(), static_cast<size_t>(10)));
    u_strncpy(apiName + 10, m_library.getUChars(), std::min(m_library.getLength(), static_cast<size_t>(10)));
    return Text(apiName, 20);
}

Text QsysObjectName::toDisplayName() const
{
    if (isEmpty())
        return Text();
    UChar displayName[22];
    u_strncpy(displayName, m_library.getUChars(),
        std::min(m_library.getLength(), static_cast<size_t>(10)));
    displayName[m_library.getLength()] = 0x002f;
    u_strncpy(displayName + m_library.getLength() + 1, m_object.getUChars(),
        std::min(m_object.getLength(), static_cast<size_t>(10)));
    return Text(displayName, m_library.getLength() + 1 + m_object.getLength());
}

IfsObjectName QsysObjectName::toIfsName() const
{
    Text result;
    Text slash(SMILE_WSTR("/"));
    if (!m_aspName.isEmpty() && m_aspName != Text(SMILE_WSTR("*SYSBAS")))
        result += slash + m_aspName;
    result += Text(SMILE_WSTR("/QSYS.LIB"));
    if (m_library != Text(SMILE_WSTR("QSYS")))
        result += slash + m_library + Text(SMILE_WSTR(".LIB"));
    if (!m_object.isEmpty())
    {
        result += slash + m_object + Text(SMILE_WSTR(".")) + m_type.subText(1);
        if (!m_member.isEmpty())
            result += slash + m_member + Text(SMILE_WSTR(".MBR"));
    }
    return IfsObjectName(result);
}

}
