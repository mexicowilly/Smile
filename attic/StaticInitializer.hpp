#if !defined(SMILE_STATICINITIALIZER_HPP__)
#define SMILE_STATICINITIALZIER_HPP__

#include "OS400ValueGroupMember.hpp"
#include "smile/SystemValueGroup.hpp"
#include <map>
#include <string>
#include <vector>

namespace smile
{

class Logger;
class NationalLanguageVersion;
class OS400ValueManipulator;
class ServicePool;
class SignonTextConverter;
class System;

class StaticInitializer
{
public:
    static StaticInitializer& getInstance();

    ~StaticInitializer();

    std::map<std::string, Logger*>& getAllLoggers();
    std::vector<OS400ValueGroupMember>& getAllNetworkAttributes();
    NationalLanguageVersion& getNationalLanguageVersion();
    OS400ValueManipulator& getOS400ValueManipulator();
    ServicePool& getServicePool();
    SignonTextConverter& getSignonTextConverter();
    System& getSystem();
    std::multimap<SystemValueGroup::SystemGroup, OS400ValueGroupMember>& getSystemValueGroups();
    std::map<SystemValueGroup::SystemGroup, Text>& getSystemValueGroupNames();

private:
    StaticInitializer();

    static StaticInitializer sm_instance;

    std::map<std::string, Logger*>* m_allLoggers;
    System* m_system;
    std::multimap<SystemValueGroup::SystemGroup, OS400ValueGroupMember>* m_systemValueGroups;
    std::map<SystemValueGroup::SystemGroup, Text>* m_systemValueGroupNames;
    std::vector<OS400ValueGroupMember>* m_allNetworkAttributes;
    ServicePool* m_servicePool;
    OS400ValueManipulator* m_os400ValueManipulator;
    NationalLanguageVersion* m_nationalLanguageVersion;
    SignonTextConverter* m_signonTextConverter;
};

inline std::map<std::string, Logger*>& StaticInitializer::getAllLoggers()
{
    return *m_allLoggers;
}

inline std::vector<OS400ValueGroupMember>& StaticInitializer::getAllNetworkAttributes()
{
    return *m_allNetworkAttributes;
}

inline NationalLanguageVersion& StaticInitializer::getNationalLanguageVersion()
{
    return *m_nationalLanguageVersion;
}

inline OS400ValueManipulator& StaticInitializer::getOS400ValueManipulator()
{
    return *m_os400ValueManipulator;
}

inline ServicePool& StaticInitializer::getServicePool()
{
    return *m_servicePool;
}

inline SignonTextConverter& StaticInitializer::getSignonTextConverter()
{
    return *m_signonTextConverter;
}

inline System& StaticInitializer::getSystem()
{
    return *m_system;
}

inline std::multimap<SystemValueGroup::SystemGroup, OS400ValueGroupMember>&
StaticInitializer::getSystemValueGroups()
{
    return *m_systemValueGroups;
}

inline std::map<SystemValueGroup::SystemGroup, Text>&
StaticInitializer::getSystemValueGroupNames()
{
    return *m_systemValueGroupNames;
}

}

#endif
