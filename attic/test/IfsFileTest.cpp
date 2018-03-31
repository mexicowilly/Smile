#include "IfsFileTest.hpp"
#include <smile/IfsFile.hpp>
#include <smile/Time.hpp>
#include <smile/Command.hpp>
#include "TestEnvironment.hpp"
#include <unicode/ucal.h>
#include <iostream>

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

void IfsFileTest::testDuplicate()
{
    smile::Text fileName("/QOpenSys/usr/bin/qsh", "US-ASCII");
    smile::IfsFile file(*m_as400, fileName);
    smile::IfsObject* dup = file.duplicate();
    std::auto_ptr<smile::IfsObject> autoDup(dup);
    predicate(HERE, file == *dup);
}

void IfsFileTest::testExists()
{
    smile::Text fileName("/QOpenSys/usr/bin/qsh", "US-ASCII");
    smile::IfsFile file(*m_as400, fileName);
    bool exists = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = file.exists());
    predicate(HERE, exists);
    fileName = smile::Text("/My/dog/has/fleas", "US-ASCII");
    file = smile::IfsFile(*m_as400, fileName);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, exists = file.exists());
    predicate(HERE, !exists);
}

void IfsFileTest::testGetAttributes()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text fileName(props.get("smile.test.existingFile").c_str(), "US-ASCII");
    smile::IfsFile file(*m_as400, fileName);
    smile::Time time;
    uint64_t int64;
    smile::AuditMode auditMode;
    smile::Text text;
    std::vector<uint8_t> bytes;
    smile::QsysObjectName qsys;
    smile::IfsObjectName ifsName;
    bool boolean;
    smile::IfsFile::StorageOption storageOption;
    smile::IfsFile::FileFormat fileFormat;
    smile::IfsFile::ScanMode scanMode;
    smile::IfsFile::ScanStatus scanStatus;
    smile::IfsFile::SystemUse systemUse;
    smile::IfsFile::UdfsDefaultFormat udfsFormat;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getAccessTime());
    std::cout << "Access time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getAllocatedSize());
    std::cout << "Allocated size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, auditMode = file.getAuditMode());
    std::cout << "Audit mode: " << auditMode << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = file.getAuthorizationListName());
    std::cout << "Authorization list name: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getAuxiliaryStoragePool());
    std::cout << "Auxiliary storage pool: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getCCSID());
    std::cout << "CCSID: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getChangeTime());
    std::cout << "Change time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getCheckedOutTime());
    std::cout << "Checked out time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = file.getCheckedOutUser());
    std::cout << "Checked out user: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getCodePage());
    std::cout << "Code page: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getCreateTime());
    std::cout << "Create time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getDaysUsedCount());
    std::cout << "Days used: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getExtendedAttributeSize());
    std::cout << "Extended attribute size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, bytes = file.getFileId());
    std::cout << "File ID: " << formatBytes(bytes) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, bytes = file.getJournalId());
    std::cout << "Journal ID: " << formatBytes(bytes) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, qsys = file.getJournalName());
    std::cout << "Journal name: " << qsys << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getJournalStartTime());
    std::cout << "Journal start time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getLastUsedTime());
    std::cout << "Last used time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getModifyTime());
    std::cout << "Modify time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, ifsName = file.getName());
    std::cout << "Name: " << ifsName << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = file.getObjectType());
    std::cout << "Object type: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, time = file.getResetTime());
    std::cout << "Reset time: " << smile::Text(time) << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, int64 = file.getSize());
    std::cout << "Size: " << int64 << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, qsys = file.getStartingJournalReceiverForApply());
    std::cout << "Starting journal receiver for apply: " << qsys << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, text = file.getStartingJournalReceiverAsp());
    std::cout << "Starting journal receiver ASP: " << text << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.hasChanged());
    std::cout << "Has changed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isApplyJournaledChangesRequired());
    std::cout << "Apply journaled changes required: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isCheckedOut());
    std::cout << "Is checked out: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isJournaled());
    std::cout << "Is journaled: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isPCArchive());
    std::cout << "Is PC archive: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isPCHidden());
    std::cout << "Is PC hidden: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isPCReadOnly());
    std::cout << "Is PC read only: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isPCSystem());
    std::cout << "Is PC system: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSaveAllowed());
    std::cout << "Is save allowed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isStorageFree());
    std::cout << "Is storage free: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isStoredLocally());
    std::cout << "Is stored locally: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.journalsAfterImage());
    std::cout << "Journals after image: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.journalsBeforeImage());
    std::cout << "Journals before image: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.journalsOptionalEntries());
    std::cout << "Journals optional entries: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.wasRollbackEnded());
    std::cout << "Was rollback ended: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, storageOption = file.getAuxiliaryStorageOption());
    std::cout << "Auxiliary storage option: " << storageOption << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, fileFormat = file.getFileFormat());
    std::cout << "File format: " << fileFormat << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, storageOption = file.getMainStorageOption());
    std::cout << "Main storage option: " << storageOption << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, scanMode = file.getScanMode());
    std::cout << "Scan mode: " << scanMode << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, scanStatus = file.getScanStatus());
    std::cout << "Scan status: " << scanStatus << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, systemUse = file.getSystemUse());
    std::cout << "System use: " << systemUse << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, udfsFormat = file.getUdfsDefaultFormat());
    std::cout << "UDFS default format: " << udfsFormat << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.hasMultipleSignatures());
    std::cout << "Has multiple signatures: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isScanSignatureDifferent());
    std::cout << "Is scan signature different: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSetGID());
    std::cout << "Is set GID: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSetUID());
    std::cout << "Is set UID: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSharedWithWritersDuringCheckpointWrite());
    std::cout << "Is shared with writers during checkpoint write: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSigned());
    std::cout << "Is signed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.isSystemSigned());
    std::cout << "Is system signed: " << boolean << std::endl;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, boolean = file.wasScannedInBinaryMode());
    std::cout << "Was scanned in binary mode: " << boolean << std::endl;
}

void IfsFileTest::testRemove()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text fileName(props.get("smile.test.existingFile").c_str(), "US-ASCII");
    smile::IfsFile file(*m_as400, fileName);
    smile::Text commandText = smile::Text("QSH CMD('cp \"") + fileName + smile::Text("\" \"") +
        fileName + smile::Text(".copy\"')");
    smile::Command command(*m_as400, commandText);
    bool result = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = command.run());
    predicate(HERE, result);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.remove());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = file.exists());
    predicate(HERE, !result);
    commandText = smile::Text("QSH CMD('mv \"") + fileName + smile::Text(".copy\" \"") +
        fileName + smile::Text("\"')");
    command.setCommand(commandText);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = command.run());
    predicate(HERE, result);
}

void IfsFileTest::testSetAttributes()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text fileName(props.get("smile.test.existingFile").c_str(), "US-ASCII");
    smile::IfsFile file(*m_as400, fileName);
    smile::Time now = static_cast<smile::Time>(ucal_getNow());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setAccessTime(now));
    int oldInt = file.getCCSID();
    predicate(HERE, oldInt != 13488);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCCSID(13488));
    predicateEquals(HERE, 13488, file.getCCSID());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCCSID(oldInt));
    oldInt = file.getCodePage();
    predicate(HERE, oldInt != 13488);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCodePage(13488));
    predicateEquals(HERE, 13488, file.getCodePage());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCodePage(oldInt));
    smile::Time oldTime = file.getCreateTime();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCreateTime(now));
    predicateEquals(HERE, now, file.getCreateTime());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setCreateTime(oldTime));
    bool oldBool = file.hasChanged();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setHasChanged(!oldBool));
    predicateEquals(HERE, !oldBool, file.hasChanged());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setHasChanged(oldBool));
    oldBool = file.isSaveAllowed();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSaveAllowed(!oldBool));
    predicateEquals(HERE, !oldBool, file.isSaveAllowed());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSaveAllowed(oldBool));
    oldTime = file.getModifyTime();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setModifyTime(now));
    predicateEquals(HERE, now, file.getModifyTime());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setModifyTime(oldTime));
    oldBool = file.isPCArchive();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCArchive(!oldBool));
    predicateEquals(HERE, !oldBool, file.isPCArchive());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCArchive(oldBool));
    oldBool = file.isPCHidden();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCHidden(!oldBool));
    predicateEquals(HERE, !oldBool, file.isPCHidden());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCHidden(oldBool));
    oldBool = file.isPCReadOnly();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCReadOnly(!oldBool));
    predicateEquals(HERE, !oldBool, file.isPCReadOnly());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCReadOnly(oldBool));
    oldBool = file.isPCSystem();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCSystem(!oldBool));
    predicateEquals(HERE, !oldBool, file.isPCSystem());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setPCSystem(oldBool));
    // Storage option ones are difficult to test, since the change doesn't take
    // effect immediately.
    /*
    smile::IFSFile::StorageOption oldStorageOption = file.getAuxiliaryStorageOption();
    predicateEquals(HERE, smile::IFSFile::StorageOption_Normal, oldStorageOption);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setAuxiliaryStorageOption(smile::IFSFile::StorageOption_Minimize));
    predicateEquals(HERE, smile::IFSFile::StorageOption_Minimize, oldStorageOption);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setAuxiliaryStorageOption(oldStorageOption));
    oldStorageOption = file.getMainStorageOption();
    predicateEquals(HERE, smile::IFSFile::StorageOption_Normal, oldStorageOption);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setMainStorageOption(smile::IFSFile::StorageOption_Minimize));
    predicateEquals(HERE, smile::IFSFile::StorageOption_Minimize, oldStorageOption);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setMainStorageOption(oldStorageOption));
    */
    // You have to be a privileged user to set the scan mode
    /*
    smile::IFSObject::ScanMode oldScanMode = file.getScanMode();
    predicateEquals(HERE, smile::IFSObject::ScanMode_Yes, oldScanMode);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setScanMode(smile::IFSObject::ScanMode_No));
    predicateEquals(HERE, smile::IFSObject::ScanMode_No, file.getScanMode());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setScanMode(oldScanMode));
    */
    oldBool = file.isSetGID();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSetGID(!oldBool));
    predicateEquals(HERE, !oldBool, file.isSetGID());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSetGID(oldBool));
    oldBool = file.isSetUID();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSetUID(!oldBool));
    predicateEquals(HERE, !oldBool, file.isSetUID());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSetUID(oldBool));
    oldBool = file.isSharedWithWritersDuringCheckpointWrite();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSharedWithWritersDuringCheckpointWrite(!oldBool));
    predicateEquals(HERE, !oldBool, file.isSharedWithWritersDuringCheckpointWrite());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, file.setSharedWithWritersDuringCheckpointWrite(oldBool));
}
