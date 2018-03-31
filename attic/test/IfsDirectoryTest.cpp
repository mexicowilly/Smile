#include "IfsDirectoryTest.hpp"
#include <smile/IfsDirectory.hpp>
#include <smile/IfsFile.hpp>
#include "TestEnvironment.hpp"
#include <iostream>
#include <algorithm>

namespace
{

std::string formatBytes(const std::vector<uint8_t>& bytes)
{
    std::ostringstream stream;
    stream << "{ ";
    stream.setf(std::ios::hex, std::ios::basefield);
    stream.width(2);
    stream.fill('0');
    for (std::vector<uint8_t>::const_iterator itor = bytes.begin();
         itor != bytes.end();
         itor++)
    {
        stream << static_cast<unsigned>(*itor);
    }
    stream << " }";
    return stream.str();
}

}

void IfsDirectoryTest::testCreateAndRemove()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str());
    smile::IfsDirectory dir(*m_as400, dirName);
    smile::IfsDirectory child(*m_as400, smile::IfsObjectName(dir.getName(), smile::Text("childdir")));
    bool exists = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = dir.exists());
    predicate(HERE, exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, !exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, child.create(false));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, child.remove());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, !exists);
    child = smile::IfsDirectory(*m_as400, smile::IfsObjectName(dir.getName(), "my/dog"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, !exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, child.create(true));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, child.remove());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, !exists);
    child = smile::IfsDirectory(*m_as400, smile::IfsObjectName(dir.getName(), "my"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, exists);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, child.remove());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = child.exists());
    predicate(HERE, !exists);
}

void IfsDirectoryTest::testDuplicate()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str());
    smile::IfsDirectory dir(*m_as400, dirName);
    smile::IfsObject* dup = dir.duplicate();
    std::auto_ptr<smile::IfsObject> autoDup(dup);
    predicate(HERE, dir == *dup);
}

void IfsDirectoryTest::testExists()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str());
    smile::IfsDirectory dir(*m_as400, dirName);
    bool exists = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = dir.exists());
    predicate(HERE, exists);
    dir = smile::IfsDirectory(*m_as400, smile::IfsObjectName("/QOpenSys/usr/bin/qsh"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = dir.exists());
    predicate(HERE, !exists);
    dir = smile::IfsDirectory(*m_as400, smile::IfsObjectName("/My/Dog/Has/Fleas"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = dir.exists());
    predicate(HERE, !exists);
}

void IfsDirectoryTest::testGetAttributes()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text fileName(props.get("smile.test.existingDirectory").c_str(), "US-ASCII");
    smile::IfsDirectory dir(*m_as400, fileName);
    smile::Time time;
    uint64_t int64;
    smile::AuditMode auditMode;
    smile::Text text;
    std::vector<uint8_t> bytes;
    smile::QsysObjectName qsys;
    smile::IfsObjectName ifsName;
    bool boolean;
    smile::IfsObject::ScanMode scanMode;
    smile::IfsDirectory::DirectoryFormat dirFormat;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getAccessTime());
    std::cout << "Access time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getAllocatedSize());
    std::cout << "Allocated size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, auditMode = dir.getAuditMode());
    std::cout << "Audit mode: " << auditMode << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = dir.getAuthorizationListName());
    std::cout << "Authorization list name: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getAuxiliaryStoragePool());
    std::cout << "Auxiliary storage pool: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getCCSID());
    std::cout << "CCSID: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getChangeTime());
    std::cout << "Change time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getCheckedOutTime());
    std::cout << "Checked out time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = dir.getCheckedOutUser());
    std::cout << "Checked out user: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getCodePage());
    std::cout << "Code page: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getCreateTime());
    std::cout << "Create time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getDaysUsedCount());
    std::cout << "Days used: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getExtendedAttributeSize());
    std::cout << "Extended attribute size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, bytes = dir.getFileId());
    std::cout << "Directory ID: " << formatBytes(bytes) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, bytes = dir.getJournalId());
    std::cout << "Journal ID: " << formatBytes(bytes) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, qsys = dir.getJournalName());
    std::cout << "Journal name: " << qsys << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getJournalStartTime());
    std::cout << "Journal start time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getLastUsedTime());
    std::cout << "Last used time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getModifyTime());
    std::cout << "Modify time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, ifsName = dir.getName());
    std::cout << "Name: " << ifsName << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = dir.getObjectType());
    std::cout << "Object type: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = dir.getResetTime());
    std::cout << "Reset time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = dir.getSize());
    std::cout << "Size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, qsys = dir.getStartingJournalReceiverForApply());
    std::cout << "Starting journal receiver for apply: " << qsys << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = dir.getStartingJournalReceiverAsp());
    std::cout << "Starting journal receiver ASP: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.hasChanged());
    std::cout << "Has changed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isApplyJournaledChangesRequired());
    std::cout << "Apply journaled changes required: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isCheckedOut());
    std::cout << "Is checked out: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isJournaled());
    std::cout << "Is journaled: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isPCArchive());
    std::cout << "Is PC archive: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isPCHidden());
    std::cout << "Is PC hidden: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isPCReadOnly());
    std::cout << "Is PC read only: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isPCSystem());
    std::cout << "Is PC system: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isSaveAllowed());
    std::cout << "Is save allowed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isStorageFree());
    std::cout << "Is storage free: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.isStoredLocally());
    std::cout << "Is stored locally: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.journalsAfterImage());
    std::cout << "Journals after image: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.journalsBeforeImage());
    std::cout << "Journals before image: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.journalsOptionalEntries());
    std::cout << "Journals optional entries: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.wasRollbackEnded());
    std::cout << "Was rollback ended: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, auditMode = dir.getCreateObjectAuditMode());
    std::cout << "Created object audit mode: " << auditMode << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, scanMode = dir.getCreateObjectScanMode());
    std::cout << "Created object scan mode: " << scanMode << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, dirFormat = dir.getDirectoryFormat());
    std::cout << "Directory format: " << dirFormat << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.hasRestrictedRenamesAndUnlinks());
    std::cout << "Has restricted renames and unlinks: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = dir.journalsNewSubtrees());
    std::cout << "Journals new subtrees: " << boolean << std::endl;
}

void IfsDirectoryTest::testListNames()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str());
    smile::IfsDirectory dir(*m_as400, dirName);
    std::vector<smile::IfsObjectName> names;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, names = dir.listNames());
    std::cout << "In directory " << dirName << ":" << std::endl;
    for (std::vector<smile::IfsObjectName>::iterator itor = names.begin();
         itor != names.end();
         itor++)
    {
        std::cout << *itor << std::endl;
    }
    predicateEquals(HERE, static_cast<size_t>(3), names.size());
    predicate(HERE, std::find(names.begin(), names.end(), smile::IfsObjectName(dirName, "chunky")) != names.end());
    predicate(HERE, std::find(names.begin(), names.end(), smile::IfsObjectName(dirName, "monkey")) != names.end());
    predicate(HERE, std::find(names.begin(), names.end(), smile::IfsObjectName(dirName, "funky")) != names.end());
}

void IfsDirectoryTest::testListObjects()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str());
    smile::IfsDirectory dir(*m_as400, dirName);
    std::vector<smile::IfsObject*> objects;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, objects = dir.listObjects());
    std::cout << "In directory " << dirName << ":" << std::endl;
    for (std::vector<smile::IfsObject*>::iterator itor = objects.begin();
         itor != objects.end();
         itor++)
    {
        std::cout << (*itor)->getName() << std::endl;
    }
    predicateEquals(HERE, static_cast<size_t>(3), objects.size());
    for (std::vector<smile::IfsObject*>::iterator itor = objects.begin();
         itor != objects.end();
         itor++)
    {
        if ((*itor)->getName() == smile::IfsObjectName(dirName, "chunky") ||
            (*itor)->getName() == smile::IfsObjectName(dirName, "funky"))
        {
            predicate(HERE, dynamic_cast<smile::IfsFile*>(*itor) != 0);
        }
        else if ((*itor)->getName() == smile::IfsObjectName(dirName, "monkey"))
        {
            predicate(HERE, dynamic_cast<smile::IfsDirectory*>(*itor) != 0);
        }
        else
        {
            fail(HERE, std::string("Unexpected object type for ") +
                (*itor)->getName().getAbsolutePath().convertToDefault());
        }
        delete *itor;
    }
}
