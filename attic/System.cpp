#include <smile/System.hpp>
#include <smile/Exception.hpp>
#include "LogUtil.hpp"
#include "Config.hpp"
#include "StaticInitializer.hpp"
#include <pwd.h>
#include <errno.h>
#include <unistd.h>

namespace
{

#if defined(__APPLE__)

const char* SYSTEM_PROPERTIES = "/Library/Application Support/Smile/smile.properties";
const char* USER_PROPERTIES = "Library/Application Support/Smile/smile.properties";

#elif defined(__unix) || (SMILE_OS == aix)

const char* SYSTEM_PROPERTIES = "/etc/smile.properties";
const char* USER_PROPERTIES = ".smile.properties";

#else

#error No system property file locations defined

#endif

}

namespace smile
{

System::System()
{
    Logger& log = LogUtil::smileLogger();
    m_properties.setInt(Text(SMILE_WSTR("service.pool.inactivity.timeout")), 60 * 60);
    m_properties.setInt(Text(SMILE_WSTR("service.pool.capacity")), 10);
    m_properties.setBool(Text(SMILE_WSTR("service.pool.enabled")), true);
    m_properties.setInt(Text(SMILE_WSTR("service.pool.wait.timeout")), 5);
    m_properties.setInt(Text(SMILE_WSTR("service.thread.poll.timeout")), 100);
    if (access(SYSTEM_PROPERTIES, R_OK) == 0)
    {
        try
        {
            m_properties.merge(Properties(SYSTEM_PROPERTIES));
        }
        catch (Exception& e)
        {
            SMILE_LOG_ERROR(log, "Error parsing system properties: " << e);
        }
    }
    else
    {
        SMILE_LOG_INFO(log, "System properties not found at " << SYSTEM_PROPERTIES);
    }
    struct passwd userInfo;
    struct passwd* result;
    char buf[8 * 1024];
    if (getpwuid_r(getuid(), &userInfo, buf, sizeof(buf), &result) != 0)
        throw ErrnoException(__FILE__, __LINE__, "Could not get user information", errno);
    if (result != 0)
    {
        std::string userProperties(userInfo.pw_dir);
        if (userProperties[userProperties.length() - 1] != '/')
            userProperties += '/';
        userProperties += USER_PROPERTIES;
        if (access(userProperties.c_str(), R_OK) == 0)
        {
            try
            {
                m_properties.merge(Properties(userProperties.c_str()));
            }
            catch (Exception& e)
            {
                SMILE_LOG_ERROR(log, "Error parsing user properties: " << e);
            }
        }
        else
        {
            SMILE_LOG_INFO(log, "User properties not found at " << userProperties);
        }
    }
    if (log.isDebugEnabled())
    {
        SMILE_LOG_FORCE(LogLevel_Debug, log, "All system properties:");
        std::vector<std::pair<Text, Text> > props = m_properties.getAll();
        for (std::vector<std::pair<Text, Text> >::iterator itor = props.begin();
             itor != props.end();
             itor++)
        {
            SMILE_LOG_FORCE(LogLevel_Debug, log, "  " << itor->first << " =  " << itor->second);
        }
    }
}

System& System::getInstance()
{
    return StaticInitializer::getInstance().getSystem();
}

}
