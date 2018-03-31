#if !defined(SMILE_OS400VALUEGROUPMEMBER_HPP__)
#define SMILE_OS400VALUEGROUPMEMBER_HPP__

#include <smile/OS400Version.hpp>

namespace smile
{

class OS400ValueGroupMember
{
public:
    OS400ValueGroupMember();
    OS400ValueGroupMember(const smile::Text& name, const smile::OS400Version& earliestVersion);
    OS400ValueGroupMember(const OS400ValueGroupMember& info);

    OS400ValueGroupMember& operator= (const OS400ValueGroupMember& info);

    const smile::OS400Version& getEarliestVersion() const;
    const smile::Text& getName() const;

private:
    smile::Text m_name;
    smile::OS400Version m_earliestVersion;
};

inline OS400ValueGroupMember::OS400ValueGroupMember()
{
}

inline const smile::OS400Version& OS400ValueGroupMember::getEarliestVersion() const
{
    return m_earliestVersion;
}

inline const smile::Text& OS400ValueGroupMember::getName() const
{
    return m_name;
}

}

#endif
