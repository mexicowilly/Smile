#if !defined(SMILE_QSYSOBJECT_HPP__)
#define SMILE_QSYSOBJECT_HPP__

#include <smile/AS400.hpp>
#include <smile/Time.hpp>
#include <smile/QsysObjectName.hpp>
#include <smile/AuditMode.hpp>
#include <smile/OS400Version.hpp>

namespace smile
{

class QsysObject
{
public:
    enum Domain
    {
        Domain_User,
        Domain_System
    };

    enum Storage
    {
        Storage_Keep,
        Storage_Free
    };

    enum SaveDevice
    {
        SaveDevice_None,
        SaveDevice_SaveFile,
        SaveDevice_Diskette,
        SaveDevice_Tape,
        SaveDevice_Optical
    };

    enum Compression
    {
        Compression_Compressed,
        Compression_PermanentlyDecompressedAndCompressible,
        Compression_PermanentlyDecompressedAndNotCompressible,
        Compression_TemporarilyDecompressed,
        Compression_NotDetermined
    };

    enum SpaceAlignment
    {
        SpaceAlignment_NonOptimal,
        SpaceAlignment_Optimal,
        SpaceAlignment_None
    };

    QsysObject(AS400& as400, const QsysObjectName& objectName);
    QsysObject(const QsysObject& object);
    virtual ~QsysObject();

    QsysObject& operator= (const QsysObject& object);

    AS400& getAS400();
    AuditMode getAuditMode();
    const Text& getAuthorizedProgramAnalysisReport();
    uint32_t getAuxiliaryStoragePool();
    const Text& getAuxiliaryStoragePoolDevice();
    const Text& getAuxiliaryStoragePoolGroup();
    Time getChangeTime();
    const Text& getCompiler();
    Compression getCompression();
    Time getCreationTime();
    const Text& getCreator();
    uint32_t getDaysUsed();
    const Text& getDescription();
    Domain getDomain();
    const Text& getExtendedAttribute();
    const Text& getGroup();
    const QsysObjectName& getJournalName();
    Time getJournalStartTime();
    Time getLastUsedTime();
    uint32_t getLibraryAuxiliaryStoragePool();
    const Text& getLibraryAuxiliaryStoragePoolDevice();
    const Text& getLibraryAuxiliaryStoragePoolGroup();
    const Text& getLicensedProgram();
    const QsysObjectName& getName();
    const Text& getObjectLevel();
    const Text& getOwner();
    uint32_t getPrimaryAssociatedSpaceSize();
    const Text& getProgramTemporaryFix();
    Time getResetTime();
    Time getRestoredTime();
    Time getSaveActiveTime();
    const Text& getSaveCommand();
    SaveDevice getSaveDevice();
    uint32_t getSavedSequence();
    uint64_t getSavedSize();
    Time getSavedTime();
    const Text& getSavedVolumeId();
    const QsysObjectName& getSaveFile();
    const Text& getSaveLabel();
    uint64_t getSize();
    SpaceAlignment getSpaceAlignment();
    const QsysObjectName& getSourceFile();
    Time getSourceFileUpdatedTime();
    const Text& getSourceMember();
    const Text& getStartingJournalReceiverAspDevice();
    const Text& getStartingJournalReceiverAspGroup();
    const QsysObjectName& getStartingJournalReceiverForApply();
    Storage getStorage();
    const Text& getSystemWhereCreated();
    const Text& getUserDefinedAttribute();
    const OS400Version& getVersionWhereCreated();
    bool isAspOverflowed();
    bool isBeforeImageJournaled();
    bool isChangedByProgram();
    bool isChangedByUser();
    bool isJournaled();
    bool isMultiplySigned();
    bool isOmittingOpenCloseJournalEntries();
    bool isProgramChangeAllowed();
    bool isSigned();
    bool isSystemSigned();
    bool isUsageInformationUpdated();

    void refresh();

protected:
    void ensureData();
    virtual void resetAttributes();

private:
    AS400* m_as400;
    QsysObjectName m_name;
    uint32_t m_auxiliaryStoragePool;
    Text m_owner;
    Domain m_domain;
    Time m_creationTime;
    Time m_changeTime;
    Text m_extendedAttribute;
    Text m_description;
    QsysObjectName m_sourceFile;
    Text m_sourceMember;
    Time m_sourceFileUpdatedTime;
    Time m_savedTime;
    Time m_restoredTime;
    Text m_creator;
    Text m_systemWhereCreated;
    Time m_resetTime;
    uint64_t m_savedSize;
    uint32_t m_savedSequence;
    Storage m_storage;
    Text m_saveCommand;
    Text m_saveVolumeId;
    SaveDevice m_saveDevice;
    QsysObjectName m_saveFile;
    Text m_saveLabel;
    OS400Version m_versionWhereCreated;
    Text m_compiler;
    Text m_objectLevel;
    bool m_userChanged;
    Text m_licensedProgram;
    Text m_programTemporaryFix;
    Text m_authorizedProgramAnalysisReport;
    Time m_lastUsedTime;
    bool m_usageInformationUpdated;
    uint32_t m_daysUsed;
    uint64_t m_size;
    Compression m_compression;
    bool m_allowChangeByProgram;
    bool m_changedByProgram;
    Text m_userDefinedAttribute;
    bool m_overflowedAsp;
    Time m_saveActiveTime;
    AuditMode m_auditMode;
    Text m_group;
    bool m_journaled;
    QsysObjectName m_journalName;
    bool m_journalsBeforeImage;
    bool m_journalOmitOpenCloseEntries;
    Time m_journalStartTime;
    bool m_signed;
    uint32_t m_libraryAsp;
    Text m_aspDevice;
    Text m_libraryAspDevice;
    bool m_systemSigned;
    bool m_multipleSignatures;
    uint32_t m_primaryAssociatedSpaceSize;
    SpaceAlignment m_spaceAlignment;
    Text m_auxiliaryStoragePoolGroup;
    Text m_libraryAspGroup;
    QsysObjectName m_startingJournalReceiverForApply;
    Text m_startingJournalReceiverAspDevice;
    Text m_startingJournalReceiverAspGroup;
};

inline AS400& QsysObject::getAS400()
{
    return *m_as400;
}

inline AuditMode QsysObject::getAuditMode()
{
    ensureData();
    return m_auditMode;
}

inline const Text& QsysObject::getAuthorizedProgramAnalysisReport()
{
    ensureData();
    return m_authorizedProgramAnalysisReport;
}

inline uint32_t QsysObject::getAuxiliaryStoragePool()
{
    ensureData();
    return m_auxiliaryStoragePool;
}

inline const Text& QsysObject::getAuxiliaryStoragePoolDevice()
{
    ensureData();
    return m_aspDevice;
}

inline const Text& QsysObject::getAuxiliaryStoragePoolGroup()
{
    ensureData();
    return m_auxiliaryStoragePoolGroup;
}

inline Time QsysObject::getChangeTime()
{
    ensureData();
    return m_changeTime;
}

inline const Text& QsysObject::getCompiler()
{
    ensureData();
    return m_compiler;
}

inline QsysObject::Compression QsysObject::getCompression()
{
    ensureData();
    return m_compression;
}

inline Time QsysObject::getCreationTime()
{
    ensureData();
    return m_creationTime;
}

inline const Text& QsysObject::getCreator()
{
    ensureData();
    return m_creator;
}

inline uint32_t QsysObject::getDaysUsed()
{
    ensureData();
    return m_daysUsed;
}

inline const Text& QsysObject::getDescription()
{
    ensureData();
    return m_description;
}

inline QsysObject::Domain QsysObject::getDomain()
{
    ensureData();
    return m_domain;
}

inline const Text& QsysObject::getExtendedAttribute()
{
    ensureData();
    return m_extendedAttribute;
}

inline const Text& QsysObject::getGroup()
{
    ensureData();
    return m_group;
}

inline const QsysObjectName& QsysObject::getJournalName()
{
    ensureData();
    return m_journalName;
}

inline Time QsysObject::getJournalStartTime()
{
    ensureData();
    return m_journalStartTime;
}

inline Time QsysObject::getLastUsedTime()
{
    ensureData();
    return m_lastUsedTime;
}

inline uint32_t QsysObject::getLibraryAuxiliaryStoragePool()
{
    ensureData();
    return m_libraryAsp;
}

inline const Text& QsysObject::getLibraryAuxiliaryStoragePoolDevice()
{
    ensureData();
    return m_libraryAspDevice;
}

inline const Text& QsysObject::getLibraryAuxiliaryStoragePoolGroup()
{
    ensureData();
    return m_libraryAspGroup;
}

inline const Text& QsysObject::getLicensedProgram()
{
    ensureData();
    return m_licensedProgram;
}

inline const QsysObjectName& QsysObject::getName()
{
    return m_name;
}

inline const Text& QsysObject::getObjectLevel()
{
    ensureData();
    return m_objectLevel;
}

inline const Text& QsysObject::getOwner()
{
    return m_owner;
}

inline uint32_t QsysObject::getPrimaryAssociatedSpaceSize()
{
    ensureData();
    return m_primaryAssociatedSpaceSize;
}

inline const Text& QsysObject::getProgramTemporaryFix()
{
    ensureData();
    return m_programTemporaryFix;
}

inline Time QsysObject::getResetTime()
{
    ensureData();
    return m_resetTime;
}

inline Time QsysObject::getRestoredTime()
{
    ensureData();
    return m_restoredTime;
}

inline Time QsysObject::getSaveActiveTime()
{
    ensureData();
    return m_saveActiveTime;
}

inline const Text& QsysObject::getSaveCommand()
{
    ensureData();
    return m_saveCommand;
}

inline QsysObject::SaveDevice QsysObject::getSaveDevice()
{
    ensureData();
    return m_saveDevice;
}

inline uint32_t QsysObject::getSavedSequence()
{
    ensureData();
    return m_savedSequence;
}

inline uint64_t QsysObject::getSavedSize()
{
    ensureData();
    return m_savedSize;
}

inline Time QsysObject::getSavedTime()
{
    ensureData();
    return m_savedTime;
}

inline const Text& QsysObject::getSavedVolumeId()
{
    ensureData();
    return m_saveVolumeId;
}

inline const QsysObjectName& QsysObject::getSaveFile()
{
    ensureData();
    return m_saveFile;
}

inline const Text& QsysObject::getSaveLabel()
{
    ensureData();
    return m_saveLabel;
}

inline uint64_t QsysObject::getSize()
{
    ensureData();
    return m_size;
}

inline const QsysObjectName& QsysObject::getSourceFile()
{
    ensureData();
    return m_sourceFile;
}

inline Time QsysObject::getSourceFileUpdatedTime()
{
    ensureData();
    return m_sourceFileUpdatedTime;
}

inline const Text& QsysObject::getSourceMember()
{
    ensureData();
    return m_sourceMember;
}

inline QsysObject::SpaceAlignment QsysObject::getSpaceAlignment()
{
    ensureData();
    return m_spaceAlignment;
}

inline const Text& QsysObject::getStartingJournalReceiverAspDevice()
{
    ensureData();
    return m_startingJournalReceiverAspDevice;
}

inline const Text& QsysObject::getStartingJournalReceiverAspGroup()
{
    ensureData();
    return m_startingJournalReceiverAspGroup;
}

inline const QsysObjectName& QsysObject::getStartingJournalReceiverForApply()
{
    ensureData();
    return m_startingJournalReceiverForApply;
}

inline QsysObject::Storage QsysObject::getStorage()
{
    ensureData();
    return m_storage;
}

inline const Text& QsysObject::getSystemWhereCreated()
{
    ensureData();
    return m_systemWhereCreated;
}

inline const Text& QsysObject::getUserDefinedAttribute()
{
    ensureData();
    return m_userDefinedAttribute;
}

inline const OS400Version& QsysObject::getVersionWhereCreated()
{
    ensureData();
    return m_versionWhereCreated;
}

inline bool QsysObject::isAspOverflowed()
{
    ensureData();
    return m_overflowedAsp;
}

inline bool QsysObject::isBeforeImageJournaled()
{
    ensureData();
    return m_journalsBeforeImage;
}

inline bool QsysObject::isChangedByProgram()
{
    ensureData();
    return m_changedByProgram;
}

inline bool QsysObject::isChangedByUser()
{
    ensureData();
    return m_userChanged;
}

inline bool QsysObject::isJournaled()
{
    ensureData();
    return m_journaled;
}

inline bool QsysObject::isMultiplySigned()
{
    ensureData();
    return m_multipleSignatures;
}

inline bool QsysObject::isOmittingOpenCloseJournalEntries()
{
    ensureData();
    return m_journalOmitOpenCloseEntries;
}

inline bool QsysObject::isProgramChangeAllowed()
{
    ensureData();
    return m_allowChangeByProgram;
}

inline bool QsysObject::isSigned()
{
    ensureData();
    return m_signed;
}

inline bool QsysObject::isSystemSigned()
{
    ensureData();
    return m_systemSigned;
}

inline bool QsysObject::isUsageInformationUpdated()
{
    ensureData();
    return m_usageInformationUpdated;
}

}

#endif
