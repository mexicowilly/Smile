#include <smile/IfsObjectName.hpp>
#include <smile/Exception.hpp>
#include <assert.h>

std::ostream& operator<< (std::ostream& stream, const smile::IfsObjectName& ifsname)
{
    stream << ifsname.getAbsolutePath();
    return stream;
}

namespace
{

const UChar SLASH_CHAR = 0x002f;
const smile::Text SLASH(&SLASH_CHAR, 1);

smile::Text cleanseName(const smile::Text& name)
{
    if (name.getLength() == 0)
    {
        throw smile::InvalidStateException(__FILE__, __LINE__,
                                           "The object name is empty");
    }
    if (name.getAt(0) != 0x002f)
    {
        throw smile::InvalidStateException(__FILE__, __LINE__,
                                           "IFS object names must be absolute");
    }
    if (name.getLength() == 1)
        return smile::Text(name);
    UChar dotChar = 0x002e;
    UChar dotdotChars[] = { 0x002e, 0x002e };
    smile::Text dot(&dotChar, 1);
    smile::Text dotdot(dotdotChars, 2);
    std::vector<smile::Text> tokens = name.tokenize(SLASH);
    std::vector<smile::Text*> finalElements;
    for (std::vector<smile::Text>::iterator itor = tokens.begin();
         itor != tokens.end();
         itor++)
    {
        if (*itor == dot)
        {
            continue;
        }
        else if (*itor == dotdot)
        {
            if (finalElements.empty())
                throw smile::InvalidStateException(__FILE__, __LINE__, "Invalid path name");
            finalElements.pop_back();
        }
        else
        {
            finalElements.push_back(&(*itor));
        }
    }
    UChar buf[8192];
    size_t length = 0;
    for (std::vector<smile::Text*>::iterator itor = finalElements.begin();
         itor != finalElements.end();
         itor++)
    {
        buf[length++] = 0x002f;
        u_strncpy(buf + length, (*itor)->getUChars(), (*itor)->getLength());
        length += (*itor)->getLength();
    }
    return smile::Text(buf, length);
}

}

namespace smile
{

IfsObjectName::IfsObjectName()
    : m_path(SMILE_WSTR("/"))
{
}

IfsObjectName::IfsObjectName(const Text& path)
    : m_path(cleanseName(path))
{
}

IfsObjectName::IfsObjectName(const Text& parent, const Text& object)
    : m_path(cleanseName(parent))
{
    m_path += SLASH + object;
}

IfsObjectName::IfsObjectName(const IfsObjectName& name)
    : m_path(name.m_path)
{
}

IfsObjectName::IfsObjectName(const IfsObjectName& parent, const Text& object)
    : m_path(parent.m_path)
{
    m_path += SLASH + object;
}

IfsObjectName& IfsObjectName::operator= (const IfsObjectName& name)
{
    m_path = name.m_path;
    return *this;
}

Text IfsObjectName::getObjectName() const
{
    if (m_path.getLength() == 0)
        return Text();
    if (m_path.getLength() == 1)
        return Text(m_path);
    unsigned pos = m_path.findLast(0x002f);
    assert(pos != Text::npos);
    assert(pos + 1 < m_path.getLength());
    return m_path.subText(pos + 1);
}

IfsObjectName IfsObjectName::getParentName() const
{
    IfsObjectName parent;
    if (m_path.getLength() < 2)
        return parent;
    unsigned pos = m_path.findLast(0x002f);
    assert(pos != Text::npos);
    if (pos == 0)
        pos = 1;
    parent.m_path = m_path.subText(0, pos);
    return parent;
}

}
