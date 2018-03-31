#include <smile/SystemValueGroup.hpp>
#include <smile/Exception.hpp>
#include "OS400ValueManipulator.hpp"
#include "StaticInitializer.hpp"
#include <algorithm>

namespace
{

pthread_once_t groupMapOnce = PTHREAD_ONCE_INIT;

void initializeGroupMap()
{
    std::map<smile::SystemValueGroup::SystemGroup, smile::Text>& systemGroupNameMap =
        smile::StaticInitializer::getInstance().getSystemValueGroupNames();
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_ALL] = smile::Text(SMILE_WSTR("*ALL"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_ALC] = smile::Text(SMILE_WSTR("*ALC"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_DATTIM] = smile::Text(SMILE_WSTR("*DATTIM"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_EDT] = smile::Text(SMILE_WSTR("*EDT"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_LIBL] = smile::Text(SMILE_WSTR("*LIBL"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_MSG] = smile::Text(SMILE_WSTR("*MSG"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_SEC] = smile::Text(SMILE_WSTR("*SEC"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_STG] = smile::Text(SMILE_WSTR("*STG"));
    systemGroupNameMap[smile::SystemValueGroup::SystemGroup_SYSCTL] = smile::Text(SMILE_WSTR("*SYSCTL"));

    // You might be inclined to use std::make_pair here, but you would
    // be wrong. Sun's C++ library doesn't like the first member of the
    // pair coming out as non-const.
    typedef std::pair<const smile::SystemValueGroup::SystemGroup,
                      smile::OS400ValueGroupMember> InsertionPair;
    std::multimap<smile::SystemValueGroup::SystemGroup, smile::OS400ValueGroupMember>&
        systemGroupMap = smile::StaticInitializer::getInstance().getSystemValueGroups();
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QACTJOB"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QADLACTJ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QADLSPLA"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QADLTOTJ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QJOBMSGQFL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QJOBMSGQMX"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QJOBMSGQSZ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QJOBMSGQTL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QJOBSPLA"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QRCLSPLSTG"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTOTJOB"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMAXJOB"), 0x00050100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMAXSPLF"), 0x00050100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_ALC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSPLFACN"), 0x00050200)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCENTURY"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDATE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDAY"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDAYOFWEEK"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QHOUR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLEAPADJ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMINUTE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMONTH"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSECOND"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTIME"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QUTCOFFSET"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QYEAR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDATETIME"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTIMADJ"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_DATTIM,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTIMZON"), 0x00050300)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_EDT,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCURSYM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_EDT,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDATFMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_EDT,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDATSEP"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_EDT,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDECFMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_EDT,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTIMSEP"), 0x00040200)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_LIBL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSYSLIBL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_LIBL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QUSRLIBL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_LIBL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLIBLCKLVL"), 0x00050100)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QACGLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QHSTLOGSIZ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRBFTR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRBHLDITV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRTTXT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSFWERRLOG"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSRVDMP"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSTSMSG"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCFGMSGQ"), 0x00040400)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_MSG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLOGOUTPUT"), 0x00050400)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QALWOBJRST"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QALWUSRDMN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUDCTL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUDENDACN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUDFRCLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUDLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCRTAUT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCRTOBJAUD"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDSPSGNINF"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QFRCCVNRST"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QINACTITV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QINACTMSGQ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLMTDEVSSN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLMTSECOFR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMAXSGNACN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMAXSIGN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDEXPITV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDLMTAJC"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDLMTCHR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDLMTREP"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDMAXLEN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDMINLEN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDPOSDIF"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDRQDDGT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDRQDDIF"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDVLDPGM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QRETSVRSEC"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QRMTSIGN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSECURITY"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QUSEADPAUT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QVFYOBJRST"), 0x00050100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSHRMEMCTL"), 0x00050100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDLVL"), 0x00050100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUDLVL2"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSCANFS"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSCANFSCTL"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDRULES"), 0x00060100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDCHGBLK"), 0x00060100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWDEXPWRN"), 0x00060100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSSLPCL"), 0x00060100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSSLCSLCTL"), 0x00060100)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SEC,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSSLCSL"), 0x00060100)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QBASACTLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QBASPOOL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMAXACTLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMCHPOOL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSTGLOWACN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSTGLOWLMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_STG,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTSEPOOL"), 0x00040200)));

    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QABNORMSW"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QASTLVL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QATNPGM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUTOCFG"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUTORMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUTOSPRPT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QAUTOVRT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QBOOKPATH"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCCSID"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCHRID"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCMNARB"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCMNRCYLMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCNTRYID"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCONSOLE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCTLSBSD"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDBRCVYWT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDEVNAMING"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDEVRCYACN"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDSCJOBITV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDYNPTYSCD"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIGC"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIGCCDEFNT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIPLDATTIM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIPLSTS"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIPLTYPE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QKBDBUF"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QKBDTYPE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLANGID"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QLOCALE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMODEL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPASTHRSVR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPFRADJ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRTDEV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRTKEYFMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWRDWNLMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPWRRSTIPL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QQRYDEGREE"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QQRYTIMLMT"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QRMTIPL"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QRMTSRVATR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSCPFCONS"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSETJOBATR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSPCENV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSRLNBR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSRTSEQ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSTRPRTWTR"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSTRUPPGM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSVRAUTITV"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QUPSDLYTIM"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QUPSMSGQ"), 0x00040200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QCHRIDCTL"), 0x00040300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDYNPTYADJ"), 0x00040300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QIGCFNTSIZ"), 0x00040300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRCMLTTSK"), 0x00040300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QPRCFEAT"), 0x00040300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QMLTTHDACN"), 0x00040400)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QDBFSTCCOL"), 0x00050200)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QENDJOBLMT"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QSAVACCPTH"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTHDRSCAFN"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QTHDRSCADJ"), 0x00050300)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QALWJOBITP"), 0x00050400)));
    systemGroupMap.insert(InsertionPair(smile::SystemValueGroup::SystemGroup_SYSCTL,
        smile::OS400ValueGroupMember(SMILE_WSTR("QALWJOBITP"), 0x00050400)));
}

}

namespace smile
{

SystemValueGroup::SystemValueGroup(AS400& as400, SystemGroup group)
{
    pthread_once(&groupMapOnce, initializeGroupMap);
    std::multimap<smile::SystemValueGroup::SystemGroup, smile::OS400ValueGroupMember>&
        systemGroupMap = StaticInitializer::getInstance().getSystemValueGroups();
    std::pair<std::multimap<SystemValueGroup::SystemGroup,
                            OS400ValueGroupMember>::iterator,
              std::multimap<SystemValueGroup::SystemGroup,
                            OS400ValueGroupMember>::iterator> range =
    (group == SystemGroup_ALL) ?
        std::make_pair(systemGroupMap.begin(), systemGroupMap.end()) :
        systemGroupMap.equal_range(group);
    std::vector<Text> names;
    OS400Version systemVersion = as400.getVersion();
    for ( ; range.first != range.second; range.first++)
    {
        if (range.first->second.getEarliestVersion() <= systemVersion)
            names.push_back(range.first->second.getName());
    }
    std::map<Text, OS400GenericValue*> values =
        OS400ValueManipulator::getInstance().retrieveValues(as400, names, true);
    for (std::map<Text, OS400GenericValue*>::iterator itor = values.begin();
         itor != values.end();
         itor++)
    {
        m_systemValues.push_back(SystemValue(as400, itor->first, itor->second));
    }
    std::sort(m_systemValues.begin(), m_systemValues.end());
    m_name = StaticInitializer::getInstance().getSystemValueGroupNames()[group];
}

SystemValueGroup::SystemValueGroup(AS400& as400,
                                   const Text& groupName,
                                   const std::vector<Text>& valueNames)
    : m_name(groupName)
{
    std::map<Text, OS400GenericValue*> values =
        OS400ValueManipulator::getInstance().retrieveValues(as400, valueNames, true);
    for (std::map<Text, OS400GenericValue*>::iterator itor = values.begin();
         itor != values.end();
         itor++)
    {
        m_systemValues.push_back(SystemValue(as400, itor->first, itor->second));
    }
}

SystemValueGroup& SystemValueGroup::operator= (const SystemValueGroup& group)
{
    if (&group != this)
    {
        m_name = group.m_name;
        m_systemValues = group.m_systemValues;
    }
    return *this;
}

const SystemValue& SystemValueGroup::operator[] (const Text& name) const
{
    if (m_systemValues.empty())
        throw InvalidStateException(__FILE__, __LINE__, "The group is empty");
    SystemValue dummy(m_systemValues.front().getAS400(), name);
    std::vector<SystemValue>::const_iterator found =
        std::lower_bound(m_systemValues.begin(), m_systemValues.end(), dummy);
    if (found == m_systemValues.end() || found->getName() != name)
    {
        std::ostringstream stream;
        stream << name << " was not found";
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
    return *found;
}

}
