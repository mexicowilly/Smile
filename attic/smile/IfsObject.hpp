#if !defined(SMILE_IFSOBJECT_HPP__)
#define SMILE_IFSOBJECT_HPP__

#include <smile/QsysObjectName.hpp>
#include <smile/Time.hpp>
#include <smile/AS400.hpp>
#include <smile/IfsObjectName.hpp>
#include <smile/AuditMode.hpp>
#include <smile/Duplicable.hpp>
#include <map>

namespace smile
{

class FileClient;

class IfsObject : public Duplicable<IfsObject>
{
public:
    enum ScanMode
    {
        ScanMode_No         = 0,
        ScanMode_Yes        = 1,
        ScanMode_ChangeOnly = 2
    };

    virtual ~IfsObject();

    bool operator== (const IfsObject& object) const;
    bool operator!= (const IfsObject& object) const;

    virtual bool exists() = 0;
    Time getAccessTime();
    uint64_t getAllocatedSize();
    AS400& getAS400();
    AuditMode getAuditMode();
    const Text& getAuthorizationListName();
    uint16_t getAuxiliaryStoragePool();
    int getCCSID();
    Time getChangeTime();
    Time getCheckedOutTime();
    const Text& getCheckedOutUser();
    int getCodePage();
    Time getCreateTime();
    size_t getDaysUsedCount();
    size_t getExtendedAttributeSize();
    const std::vector<uint8_t>& getFileId();
    const std::vector<uint8_t>& getJournalId();
    const QsysObjectName& getJournalName();
    Time getJournalStartTime();
    Time getLastUsedTime();
    Time getModifyTime();
    const IfsObjectName& getName();
    const Text& getObjectType();
    Time getResetTime();
    uint64_t getSize();
    const QsysObjectName& getStartingJournalReceiverForApply();
    const Text& getStartingJournalReceiverAsp();
    bool hasChanged();
    bool isApplyJournaledChangesRequired();
    bool isCheckedOut();
    bool isJournaled();
    bool isPCArchive();
    bool isPCHidden();
    bool isPCReadOnly();
    bool isPCSystem();
    bool isSaveAllowed();
    bool isStorageFree();
    bool isStoredLocally();
    bool journalsAfterImage();
    bool journalsBeforeImage();
    bool journalsOptionalEntries();
    void refresh();
    virtual void remove() = 0;
    void reset();
    void setAccessTime(Time time);
    void setCCSID(int ccsid);
    void setCodePage(int codePage);
    void setCreateTime(Time time);
    void setHasChanged(bool hasChanged);
    void setSaveAllowed(bool saveAllowed);
    void setModifyTime(Time time);
    void setPCArchive(bool archive);
    void setPCHidden(bool hidden);
    void setPCReadOnly(bool readOnly);
    void setPCSystem(bool system);
    bool wasRollbackEnded();

protected:
    typedef std::map<uint32_t, std::vector<uint8_t> > AttributeMap;

    IfsObject(AS400& as400, const IfsObjectName& name);
    IfsObject(const IfsObject& object);

    IfsObject& operator= (const IfsObject& object);

    void ensureData();
    std::vector<uint8_t> extractAttribute(const std::vector<uint8_t>& attrs, uint32_t attrId);
    FileClient& getFileClient();
    virtual void resetAttributes();
    void setAttribute(uint32_t attrId, const std::vector<uint8_t>& data);
    virtual void updateAttributes(const AttributeMap& attrMap, int ccsid);

private:
    AttributeMap extractAttributes(const std::vector<uint8_t>& rawAttrs) const;
    std::pair<std::vector<uint8_t>, int> retrieveAttributes();

    FileClient* m_fileClient;
    Time m_accessTime;
    uint64_t m_allocatedSize;
    AuditMode m_auditMode;
    Text m_authorizationList;
    uint16_t m_asp;
    int m_ccsid;
    Time m_changeTime;
    Time m_checkedOutTime;
    Text m_checkedOutUser;
    int m_codePage;
    Time m_createTime;
    size_t m_daysUsed;
    size_t m_extendedAttributeSize;
    std::vector<uint8_t> m_fileId;
    std::vector<uint8_t> m_journalId;
    QsysObjectName m_journalName;
    Time m_journalStartTime;
    Time m_lastUsedTime;
    Time m_modifyTime;
    Text m_objectType;
    Time m_resetTime;
    uint64_t m_size;
    QsysObjectName m_startingJournalReceiverForApply;
    Text m_startingJournalReceiverAsp;
    bool m_hasChanged;
    bool m_applyJournalChangesRequired;
    bool m_checkedOut;
    bool m_journaled;
    bool m_pcArchive;
    bool m_pcHidden;
    bool m_pcReadOnly;
    bool m_pcSystem;
    bool m_saveAllowed;
    bool m_storageFree;
    bool m_storedLocally;
    bool m_journalsAfterImage;
    bool m_journalsBeforeImage;
    bool m_journalsOptionalEntries;
    IfsObjectName m_name;
    AS400* m_as400;
    Text m_owner;
    Text m_primaryGroup;
    bool m_wasRollbackEnded;
};

inline bool IfsObject::operator!= (const IfsObject& object) const
{
    return !operator==(object);
}

inline Time IfsObject::getAccessTime()
{
    ensureData();
    return m_accessTime;
}

inline uint64_t IfsObject::getAllocatedSize()
{
    ensureData();
    return m_allocatedSize;
}

inline AS400& IfsObject::getAS400()
{
    return *m_as400;
}

inline AuditMode IfsObject::getAuditMode()
{
    ensureData();
    return m_auditMode;
}

inline const Text& IfsObject::getAuthorizationListName()
{
    ensureData();
    return m_authorizationList;
}

inline uint16_t IfsObject::getAuxiliaryStoragePool()
{
    ensureData();
    return m_asp;
}

inline int IfsObject::getCCSID()
{
    ensureData();
    return m_ccsid;
}

inline Time IfsObject::getChangeTime()
{
    ensureData();
    return m_changeTime;
}

inline Time IfsObject::getCheckedOutTime()
{
    ensureData();
    return m_checkedOutTime;
}

inline const Text& IfsObject::getCheckedOutUser()
{
    ensureData();
    return m_checkedOutUser;
}

inline int IfsObject::getCodePage()
{
    ensureData();
    return m_codePage;
}

inline Time IfsObject::getCreateTime()
{
    ensureData();
    return m_createTime;
}

inline size_t IfsObject::getDaysUsedCount()
{
    ensureData();
    return m_daysUsed;
}

inline size_t IfsObject::getExtendedAttributeSize()
{
    ensureData();
    return m_extendedAttributeSize;
}

inline const std::vector<uint8_t>& IfsObject::getFileId()
{
    ensureData();
    return m_fileId;
}

inline const std::vector<uint8_t>& IfsObject::getJournalId()
{
    ensureData();
    return m_journalId;
}

inline const QsysObjectName& IfsObject::getJournalName()
{
    ensureData();
    return m_journalName;
}

inline Time IfsObject::getJournalStartTime()
{
    ensureData();
    return m_journalStartTime;
}

inline Time IfsObject::getLastUsedTime()
{
    ensureData();
    return m_lastUsedTime;
}

inline Time IfsObject::getModifyTime()
{
    ensureData();
    return m_modifyTime;
}

inline const IfsObjectName& IfsObject::getName()
{
    return m_name;
}

inline const Text& IfsObject::getObjectType()
{
    ensureData();
    return m_objectType;
}

inline Time IfsObject::getResetTime()
{
    ensureData();
    return m_resetTime;
}

inline uint64_t IfsObject::getSize()
{
    ensureData();
    return m_size;
}

inline const QsysObjectName& IfsObject::getStartingJournalReceiverForApply()
{
    ensureData();
    return m_startingJournalReceiverForApply;
}

inline const Text& IfsObject::getStartingJournalReceiverAsp()
{
    ensureData();
    return m_startingJournalReceiverAsp;
}

inline bool IfsObject::hasChanged()
{
    ensureData();
    return m_hasChanged;
}

inline bool IfsObject::isApplyJournaledChangesRequired()
{
    ensureData();
    return m_applyJournalChangesRequired;
}

inline bool IfsObject::isCheckedOut()
{
    ensureData();
    return m_checkedOut;
}

inline bool IfsObject::isJournaled()
{
    ensureData();
    return m_journaled;
}

inline bool IfsObject::isPCArchive()
{
    ensureData();
    return m_pcArchive;
}

inline bool IfsObject::isPCHidden()
{
    ensureData();
    return m_pcHidden;
}

inline bool IfsObject::isPCReadOnly()
{
    ensureData();
    return m_pcReadOnly;
}

inline bool IfsObject::isPCSystem()
{
    ensureData();
    return m_pcSystem;
}

inline bool IfsObject::isSaveAllowed()
{
    ensureData();
    return m_saveAllowed;
}

inline bool IfsObject::isStorageFree()
{
    ensureData();
    return m_storageFree;
}

inline bool IfsObject::isStoredLocally()
{
    ensureData();
    return m_storedLocally;
}

inline bool IfsObject::journalsAfterImage()
{
    ensureData();
    return m_journalsAfterImage;
}

inline bool IfsObject::journalsBeforeImage()
{
    ensureData();
    return m_journalsBeforeImage;
}

inline bool IfsObject::journalsOptionalEntries()
{
    ensureData();
    return m_journalsOptionalEntries;
}

inline bool IfsObject::wasRollbackEnded()
{
    ensureData();
    return m_wasRollbackEnded;
}

}

#endif
