#if !defined(SMILE_IFSDIRECTORY_HPP__)
#define SMILE_IFSDIRECTORY_HPP__

#include <smile/IfsObject.hpp>

namespace smile
{

class FileClient;

class IfsDirectory : public IfsObject
{
public:
    enum DirectoryFormat
    {
        DirectoryFormat_Type1 = 0,
        DirectoryFormat_Type2 = 1
    };

    IfsDirectory(AS400& as400, const IfsObjectName& name);
    IfsDirectory(const IfsDirectory& dir);

    IfsDirectory& operator= (const IfsDirectory& dir);

    bool create(bool withParentDirectories);
    virtual IfsObject* duplicate() const;
    virtual bool exists();
    AuditMode getCreateObjectAuditMode();
    ScanMode getCreateObjectScanMode();
    DirectoryFormat getDirectoryFormat();
    bool hasRestrictedRenamesAndUnlinks();
    bool journalsNewSubtrees();
    std::vector<IfsObjectName> listNames();
    std::vector<IfsObject*> listObjects();
    virtual void remove();
    void setCreateObjectAuditingMode(AuditMode auditMode);
    void setCreateObjectScanMode(ScanMode scanMode);
    void setRestrictedRenamesAndUnlinks(bool newState);

protected:
    virtual void resetAttributes();
    virtual void updateAttributes(const AttributeMap& attrMap, int ccsid);

private:
    AuditMode m_createObjectAuditMode;
    ScanMode m_createObjectScanMode;
    DirectoryFormat m_directoryFormat;
    bool m_restrictedRenamesAndUnlinks;
    bool m_journalsNewSubtrees;
};

inline AuditMode IfsDirectory::getCreateObjectAuditMode()
{
    ensureData();
    return m_createObjectAuditMode;
}

inline IfsObject::ScanMode IfsDirectory::getCreateObjectScanMode()
{
    ensureData();
    return m_createObjectScanMode;
}

inline IfsDirectory::DirectoryFormat IfsDirectory::getDirectoryFormat()
{
    ensureData();
    return m_directoryFormat;
}

inline bool IfsDirectory::hasRestrictedRenamesAndUnlinks()
{
    ensureData();
    return m_restrictedRenamesAndUnlinks;
}

inline bool IfsDirectory::journalsNewSubtrees()
{
    ensureData();
    return m_journalsNewSubtrees;
}

}

#endif
