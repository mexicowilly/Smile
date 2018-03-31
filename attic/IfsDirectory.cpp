#include "smile/IfsDirectory.hpp"
#include "IfsAttributes.hpp"
#include "FileClient.hpp"
#include "smile/IfsFile.hpp"
#include "AttributeUtility.hpp"
#include "LogUtil.hpp"

namespace smile
{

IfsDirectory::IfsDirectory(AS400& as400, const IfsObjectName& name)
    : IfsObject(as400, name),
      m_createObjectAuditMode(AuditMode_NotAvailable),
      m_createObjectScanMode(ScanMode_No),
      m_directoryFormat(DirectoryFormat_Type1),
      m_restrictedRenamesAndUnlinks(false),
      m_journalsNewSubtrees(false)
{
}

IfsDirectory::IfsDirectory(const IfsDirectory& dir)
    : IfsObject(dir),
      m_createObjectAuditMode(dir.m_createObjectAuditMode),
      m_createObjectScanMode(dir.m_createObjectScanMode),
      m_directoryFormat(dir.m_directoryFormat),
      m_restrictedRenamesAndUnlinks(dir.m_restrictedRenamesAndUnlinks),
      m_journalsNewSubtrees(dir.m_journalsNewSubtrees)
{
}

IfsDirectory& IfsDirectory::operator= (const IfsDirectory& dir)
{
    if (&dir != this)
    {
        IfsObject::operator=(dir);
        m_createObjectAuditMode = dir.m_createObjectAuditMode;
        m_createObjectScanMode = dir.m_createObjectScanMode;
        m_directoryFormat = dir.m_directoryFormat;
        m_restrictedRenamesAndUnlinks = dir.m_restrictedRenamesAndUnlinks;
        m_journalsNewSubtrees = dir.m_journalsNewSubtrees;
    }
    return *this;
}

bool IfsDirectory::create(bool withParentDirectories)
{
    std::vector<Text> toCreate;
    IfsObjectName cur(getName());
    IfsObjectName root(SMILE_WSTR("/"));
    try
    {
        do
        {
            FileClient::Existence existence = getFileClient().exists(cur.getAbsolutePath());
            if (existence == FileClient::Existence_File)
                return false;
            else if (existence == FileClient::Existence_None)
                toCreate.push_back(cur.getAbsolutePath());
            else
                break;
            cur = cur.getParentName();
        } while (withParentDirectories && cur != root);
        for (std::vector<Text>::reverse_iterator itor = toCreate.rbegin();
             itor != toCreate.rend();
             itor++)
        {
            getFileClient().createDirectory(*itor);
        }
    }
    catch (Exception& e)
    {
        SMILE_LOG_DEBUG(LogUtil::smileLogger(),
            "Caught exception trying to create directory " << getName() <<
            ": " << e);
        return false;
    }
    return true;
}

IfsObject* IfsDirectory::duplicate() const
{
    return new IfsDirectory(*this);
}

bool IfsDirectory::exists()
{
    return getFileClient().exists(getName().getAbsolutePath()) ==
        FileClient::Existence_Directory;
}

std::vector<IfsObjectName> IfsDirectory::listNames()
{
    std::vector<std::pair<Text, bool> > contents =
        getFileClient().listDirectory(getName().getAbsolutePath());
    std::vector<IfsObjectName> output;
    for (std::vector<std::pair<Text, bool> >::iterator itor = contents.begin();
         itor != contents.end();
         itor++)
    {
        output.push_back(IfsObjectName(getName(), itor->first));
    }
    return output;
}

std::vector<IfsObject*> IfsDirectory::listObjects()
{
    std::vector<std::pair<Text, bool> > contents =
        getFileClient().listDirectory(getName().getAbsolutePath());
    std::vector<IfsObject*> output;
    for (std::vector<std::pair<Text, bool> >::iterator itor = contents.begin();
         itor != contents.end();
         itor++)
    {
        IfsObject* obj;
        if (itor->second)
            obj = new IfsDirectory(getAS400(), IfsObjectName(getName(), itor->first));
        else
            obj = new IfsFile(getAS400(), IfsObjectName(getName(), itor->first));
        output.push_back(obj);
    }
    return output;
}

void IfsDirectory::remove()
{
    getFileClient().deleteDirectory(getName().getAbsolutePath());
}

void IfsDirectory::resetAttributes()
{
    IfsObject::resetAttributes();
    m_createObjectAuditMode = AuditMode_NotAvailable;
    m_createObjectScanMode = ScanMode_No;
    m_directoryFormat = DirectoryFormat_Type1;
    m_restrictedRenamesAndUnlinks = false;
    m_journalsNewSubtrees = false;
}

void IfsDirectory::setCreateObjectAuditingMode(AuditMode auditMode)
{
    if (auditMode != AuditMode_NotAvailable)
    {
        TextConverter defaultToServer(SMILE_DEFAULT_CONVERTER, getAS400().getCCSID());
        const char* mode = "*NONE     ";
        if (auditMode == AuditMode_All)
            mode = "*ALL      ";
        else if (auditMode == AuditMode_UserProfile)
            mode = "*USRPRF   ";
        else if (auditMode == AuditMode_Change)
            mode = "*CHANGE   ";
        else
            assert(auditMode == AuditMode_None);
        setAttribute(Qp0lAttr_CRTOBJAUD,
                     defaultToServer.convert(reinterpret_cast<const uint8_t*>(mode)));
        m_createObjectAuditMode = auditMode;
    }
}

void IfsDirectory::setCreateObjectScanMode(ScanMode scanMode)
{
    std::vector<uint8_t> data;
    data.push_back(scanMode);
    setAttribute(Qp0lAttr_CRTOBJSCAN, data);
    m_createObjectScanMode = scanMode;
}

void IfsDirectory::setRestrictedRenamesAndUnlinks(bool newState)
{
    std::vector<uint8_t> data;
    data.push_back(newState ? 1 : 0);
    setAttribute(Qp0lAttr_RSTDRNMUNL, data);
    m_restrictedRenamesAndUnlinks = newState;
}

void IfsDirectory::updateAttributes(const AttributeMap& attrMap, int ccsid)
{
    static Qp0lAttr myAttrs[] =
    {
        Qp0lAttr_CRTOBJAUD, Qp0lAttr_CRTOBJSCAN, Qp0lAttr_DIR_FORMAT,
        Qp0lAttr_RSTDRNMUNL, Qp0lAttr_JOURNAL_EXTENDED_INFORMATION
    };
    static size_t myAttrCount = sizeof(myAttrs) / sizeof(myAttrs[0]);

    IfsObject::updateAttributes(attrMap, ccsid);
    for (unsigned i = 0; i < myAttrCount; i++)
    {
        AttributeMap::const_iterator found = attrMap.find(myAttrs[i]);
        if (found == attrMap.end())
            continue;
        const std::vector<uint8_t>& attr = found->second;
        if (myAttrs[i] == Qp0lAttr_CRTOBJAUD)
        {
            m_createObjectAuditMode = AttributeUtility::getAuditMode(&attr[0], ccsid);
        }
        else if (myAttrs[i] == Qp0lAttr_CRTOBJSCAN)
        {
            assert(attr.size() == 1);
            m_createObjectScanMode = static_cast<ScanMode>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_DIR_FORMAT)
        {
            assert(attr.size() == 1);
            m_directoryFormat = static_cast<DirectoryFormat>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_RSTDRNMUNL)
        {
            assert(attr.size() == 1);
            m_restrictedRenamesAndUnlinks = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_JOURNAL_EXTENDED_INFORMATION)
        {
            assert(attr.size() > 1);
            m_journalsNewSubtrees = (attr[1] & 0x80) != 0;
        }
    }
}

}
