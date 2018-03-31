#include "OS400ValueGroupMember.hpp"

namespace smile
{

OS400ValueGroupMember::OS400ValueGroupMember(const smile::Text& name, const smile::OS400Version& earliestVersion)
    : m_name(name), m_earliestVersion(earliestVersion)
{
}

OS400ValueGroupMember::OS400ValueGroupMember(const OS400ValueGroupMember& info)
    : m_name(info.m_name), m_earliestVersion(info.m_earliestVersion)
{
}

OS400ValueGroupMember& OS400ValueGroupMember::operator= (const OS400ValueGroupMember& info)
{
    m_name = info.m_name;
    m_earliestVersion = info.m_earliestVersion;
    return *this;
}

}
