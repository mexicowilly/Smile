#include <smile/NetworkAttribute.hpp>
#include "OS400ValueManipulator.hpp"
#include "StaticInitializer.hpp"

namespace
{

pthread_once_t initializeOnce = PTHREAD_ONCE_INIT;

void initializeAllAttributes()
{
    std::vector<smile::OS400ValueGroupMember>& allAttributes =
        smile::StaticInitializer::getInstance().getAllNetworkAttributes();
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRBCKFP"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRCTLD"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRDFTFP"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRFTR"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRHLDCNT"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRLOGSTS"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRPRIFP"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRRQSFP"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALRSTS"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALWADDCLU"), 0x00040400));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALWANYNET"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALWHPRTWR"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("ALWVRTAPPN"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DDMACC"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DFTCNNLST"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DFTMODE"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DFTNETTYPE"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DTACPR"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("DTACPRINM"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("HPRPTHTMR"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("JOBACN"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("LCLCPNAME"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("LCLLOCNAME"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("LCLNETID"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("MAXHOP"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("MAXINTSSN"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("MDMCNTRYID"), 0x00040400));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("MSGQ"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("NETSERVER"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("NODETYPE"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("NWSDOMAIN"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("OUTQ"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("PCSACC"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("PNDSYSNAME"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("RAR"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("SYSNAME"), 0x00040200));
    allAttributes.push_back(smile::OS400ValueGroupMember(SMILE_WSTR("VRTAUTODEV"), 0x00040200));
}

}

namespace smile
{

NetworkAttribute::NetworkAttribute(AS400& as400, const Text& name, OS400GenericValue* value)
    : OS400NameValuePair(as400, name)
{
    setValueLocally(value);
}

OS400NameValuePair* NetworkAttribute::duplicate() const
{
    return new NetworkAttribute(*this);
}

std::vector<NetworkAttribute> NetworkAttribute::getAll(AS400& as400)
{
    pthread_once(&initializeOnce, initializeAllAttributes);
    OS400Version systemVersion = as400.getVersion();
    std::vector<Text> names;
    std::vector<OS400ValueGroupMember>& allAttributes =
        StaticInitializer::getInstance().getAllNetworkAttributes();
    for (std::vector<OS400ValueGroupMember>::iterator itor = allAttributes.begin();
         itor != allAttributes.end();
         itor++)
    {
        if ((*itor).getEarliestVersion() <= systemVersion)
            names.push_back((*itor).getName());
    }
    std::map<Text, OS400GenericValue*> values =
        OS400ValueManipulator::getInstance().retrieveValues(as400, names, false);
    std::vector<NetworkAttribute> output;
    for (std::map<Text, OS400GenericValue*>::iterator itor = values.begin();
         itor != values.end();
         itor++)
    {
        output.push_back(NetworkAttribute(as400, itor->first, itor->second));
    }
    return output;
}

bool NetworkAttribute::isSystemValue() const
{
    return false;
}

}
