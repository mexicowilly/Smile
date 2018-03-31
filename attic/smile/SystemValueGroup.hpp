#if !defined(SMILE_SYSTEMVALUEGROUP_HPP__)
#define SMILE_SYSTEMVALUEGROUP_HPP__

#include <smile/SystemValue.hpp>

namespace smile
{

class SystemValueGroup
{
public:
    enum SystemGroup
    {
        SystemGroup_ALL,
        SystemGroup_ALC,
        SystemGroup_DATTIM,
        SystemGroup_EDT,
        SystemGroup_LIBL,
        SystemGroup_MSG,
        SystemGroup_SEC,
        SystemGroup_STG,
        SystemGroup_SYSCTL
    };

    SystemValueGroup(AS400& as400, SystemGroup group);
    SystemValueGroup(AS400& as400,
                     const Text& groupName,
                     const std::vector<Text>& valueNames);
    SystemValueGroup(const SystemValueGroup& group);

    SystemValueGroup& operator= (const SystemValueGroup& group);
    const SystemValue& operator[] (const Text& name) const;

    const SystemValue& getAt(size_t index) const;
    const Text& getName() const;
    size_t getSize() const;

private:
    Text m_name;
    std::vector<SystemValue> m_systemValues;
};

inline SystemValueGroup::SystemValueGroup(const SystemValueGroup& group)
    : m_name(group.m_name), m_systemValues(group.m_systemValues)
{
}

inline const SystemValue& SystemValueGroup::getAt(size_t index) const
{
    return m_systemValues[index];
}

inline const Text& SystemValueGroup::getName() const
{
    return m_name;
}

inline size_t SystemValueGroup::getSize() const
{
    return m_systemValues.size();
}

}

#endif
