#include "StaticInitializer.hpp"
#include "NationalLanguageVersion.hpp"
#include "OS400ValueManipulator.hpp"
#include "ServicePool.hpp"
#include "SignonTextConverter.hpp"
#include "smile/System.hpp"
#include "LogUtil.hpp"

namespace smile
{

StaticInitializer StaticInitializer::sm_instance;

StaticInitializer::StaticInitializer()
    : m_allLoggers(new std::map<std::string, Logger*>()),
      m_system(new System()),
      m_systemValueGroups(new std::multimap<SystemValueGroup::SystemGroup, OS400ValueGroupMember>()),
      m_systemValueGroupNames(new std::map<SystemValueGroup::SystemGroup, Text>()),
      m_allNetworkAttributes(new std::vector<OS400ValueGroupMember>()),
      m_servicePool(new ServicePool()),
      m_os400ValueManipulator(new OS400ValueManipulator()),
      m_nationalLanguageVersion(new NationalLanguageVersion()),
      m_signonTextConverter(new SignonTextConverter())
{
}

StaticInitializer::~StaticInitializer()
{
    delete m_signonTextConverter;
    delete m_nationalLanguageVersion;
    delete m_os400ValueManipulator;
    delete m_servicePool;
    delete m_allNetworkAttributes;
    delete m_systemValueGroupNames;
    delete m_systemValueGroups;
    delete m_system;
    delete m_allLoggers;
}

StaticInitializer& StaticInitializer::getInstance()
{
    return sm_instance;
}

}
