#include "smile/IfsObject.hpp"
#include "ByteOrder.hpp"
#include "IfsAttributes.hpp"
#include "Packet.hpp"
#include "FileClient.hpp"
#include "smile/ProgramParameterList.hpp"
#include "QlgPathName.hpp"
#include "smile/ServiceProgramProcedure.hpp"
#include "LogUtil.hpp"
#include "AttributeUtility.hpp"

namespace
{

const smile::Text QSYS_LIB(SMILE_WSTR("QSYS"));
const smile::Text QP0LLIB2_SRVPGM(SMILE_WSTR("QP0LLIB2"));
const smile::Text QP0LLIB3_SRVPGM(SMILE_WSTR("QP0LLIB3"));
const smile::Text QP0LGETATTR(SMILE_WSTR("Qp0lGetAttr"));
const smile::Text QP0LSETATTR(SMILE_WSTR("Qp0lSetAttr"));

struct Qp0l_Attr_Header_t
{
    uint32_t Next_Attr_Offset;
    uint32_t Attr_ID;
    uint32_t Attr_Size;
    uint8_t Reserved[4];
};

class Qp0lAttrHeader : public smile::InParameter
{
public:
    Qp0lAttrHeader(uint32_t attr,
                   const std::vector<uint8_t>& data,
                   smile::ProgramParameter::PassBy passBy = smile::ProgramParameter::PassBy_Value);
    Qp0lAttrHeader(const Qp0lAttrHeader& header);

    Qp0lAttrHeader& operator= (const Qp0lAttrHeader& header);

    virtual ProgramParameter* duplicate() const;

protected:
    virtual std::map<std::string, std::string> getAttrMap() const;
};

inline uint64_t makeTime(uint32_t secs)
{
    return static_cast<uint64_t>(secs) * UINT64_C(1000);
}

Qp0lAttrHeader::Qp0lAttrHeader(uint32_t attr,
                               const std::vector<uint8_t>& data,
                               smile::ProgramParameter::PassBy passBy)
    : ProgramParameter(passBy), InParameter(passBy)
{
    Qp0l_Attr_Header_t base;
    memset(&base, 0, sizeof(base));
    base.Attr_ID = H32_TO_N(attr);
    base.Attr_Size = H32_TO_N(data.size());
    uint8_t* baseBytes = reinterpret_cast<uint8_t*>(&base);
    std::vector<uint8_t> bytes(baseBytes, baseBytes + sizeof(base));
    bytes.insert(bytes.end(), data.begin(), data.end());
    setInData(bytes);
}

Qp0lAttrHeader::Qp0lAttrHeader(const Qp0lAttrHeader& header)
    : ProgramParameter(header), InParameter(header)
{
}

Qp0lAttrHeader& Qp0lAttrHeader::operator= (const Qp0lAttrHeader& header)
{
    InParameter::operator=(header);
    return *this;
}

smile::ProgramParameter* Qp0lAttrHeader::duplicate() const
{
    return new Qp0lAttrHeader(*this);
}

std::map<std::string, std::string> Qp0lAttrHeader::getAttrMap() const
{
    std::map<std::string, std::string> attrs = InParameter::getAttrMap();
    const std::vector<uint8_t>& inData = const_cast<Qp0lAttrHeader*>(this)->getInData();
    const Qp0l_Attr_Header_t* header = reinterpret_cast<const Qp0l_Attr_Header_t*>(&inData[0]);
    std::ostringstream stream;
    stream << "{ Next_Attr_Offset = " << N32_TO_H(header->Next_Attr_Offset) << "; "
           << " Attr_ID = " << N32_TO_H(header->Attr_ID) << "; "
           << " Attr_Size = " << N32_TO_H(header->Attr_Size) << "; "
           << " Attr_Data = { " << smile::LogUtil::bytes(
               std::vector<uint8_t>(inData.begin() + sizeof(Qp0l_Attr_Header_t), inData.end()))
           << "}; }";
    attrs["Qp0l_Attr_Header_t"] = stream.str();
    return attrs;
}

}

namespace smile
{

IfsObject::IfsObject(AS400& as400, const IfsObjectName& name)
    : m_fileClient(0),
      m_accessTime(0),
      m_allocatedSize(0),
      m_auditMode(AuditMode_NotAvailable),
      m_authorizationList(),
      m_asp(0),
      m_ccsid(-1),
      m_changeTime(0),
      m_checkedOutTime(0),
      m_checkedOutUser(),
      m_codePage(0),
      m_createTime(0),
      m_daysUsed(0),
      m_extendedAttributeSize(0),
      m_fileId(),
      m_journalId(),
      m_journalName(),
      m_journalStartTime(0),
      m_lastUsedTime(0),
      m_modifyTime(0),
      m_objectType(),
      m_resetTime(0),
      m_size(0),
      m_startingJournalReceiverForApply(),
      m_startingJournalReceiverAsp(),
      m_hasChanged(false),
      m_applyJournalChangesRequired(false),
      m_checkedOut(false),
      m_journaled(false),
      m_pcArchive(false),
      m_pcHidden(false),
      m_pcReadOnly(false),
      m_pcSystem(false),
      m_saveAllowed(false),
      m_storageFree(false),
      m_storedLocally(false),
      m_journalsAfterImage(false),
      m_journalsBeforeImage(false),
      m_journalsOptionalEntries(false),
      m_name(name),
      m_as400(&as400),
      m_owner(),
      m_primaryGroup(),
      m_wasRollbackEnded(false)
{
}

IfsObject::IfsObject(const IfsObject& object)
    : m_fileClient(0),
      m_accessTime(object.m_accessTime),
      m_allocatedSize(object.m_allocatedSize),
      m_auditMode(object.m_auditMode),
      m_authorizationList(object.m_authorizationList),
      m_asp(object.m_asp),
      m_ccsid(object.m_ccsid),
      m_changeTime(object.m_changeTime),
      m_checkedOutTime(object.m_checkedOutTime),
      m_checkedOutUser(object.m_checkedOutUser),
      m_codePage(object.m_codePage),
      m_createTime(object.m_createTime),
      m_daysUsed(object.m_daysUsed),
      m_extendedAttributeSize(object.m_extendedAttributeSize),
      m_fileId(object.m_fileId),
      m_journalId(object.m_journalId),
      m_journalName(object.m_journalName),
      m_journalStartTime(object.m_journalStartTime),
      m_lastUsedTime(object.m_lastUsedTime),
      m_modifyTime(object.m_modifyTime),
      m_objectType(object.m_objectType),
      m_resetTime(object.m_resetTime),
      m_size(object.m_size),
      m_startingJournalReceiverForApply(object.m_startingJournalReceiverForApply),
      m_startingJournalReceiverAsp(object.m_startingJournalReceiverAsp),
      m_hasChanged(object.m_hasChanged),
      m_applyJournalChangesRequired(object.m_applyJournalChangesRequired),
      m_checkedOut(object.m_checkedOut),
      m_journaled(object.m_journaled),
      m_pcArchive(object.m_pcArchive),
      m_pcHidden(object.m_pcHidden),
      m_pcReadOnly(object.m_pcReadOnly),
      m_pcSystem(object.m_pcSystem),
      m_saveAllowed(object.m_saveAllowed),
      m_storageFree(object.m_storageFree),
      m_storedLocally(object.m_storedLocally),
      m_journalsAfterImage(object.m_journalsAfterImage),
      m_journalsBeforeImage(object.m_journalsBeforeImage),
      m_journalsOptionalEntries(object.m_journalsOptionalEntries),
      m_name(object.m_name),
      m_as400(object.m_as400),
      m_owner(object.m_owner),
      m_primaryGroup(object.m_primaryGroup),
      m_wasRollbackEnded(object.m_wasRollbackEnded)
{
}

IfsObject::~IfsObject()
{
    delete m_fileClient;
}

IfsObject& IfsObject::operator= (const IfsObject& object)
{
    if (&object != this)
    {
        delete m_fileClient;
        m_fileClient = 0;
        m_accessTime = object.m_accessTime;
        m_allocatedSize = object.m_allocatedSize;
        m_auditMode = object.m_auditMode;
        m_authorizationList = object.m_authorizationList;
        m_asp = object.m_asp;
        m_ccsid = object.m_ccsid;
        m_changeTime = object.m_changeTime;
        m_checkedOutTime = object.m_checkedOutTime;
        m_checkedOutUser = object.m_checkedOutUser;
        m_codePage = object.m_codePage;
        m_createTime = object.m_createTime;
        m_daysUsed = object.m_daysUsed;
        m_extendedAttributeSize = object.m_extendedAttributeSize;
        m_fileId = object.m_fileId;
        m_journalId = object.m_journalId;
        m_journalName = object.m_journalName;
        m_journalStartTime = object.m_journalStartTime;
        m_lastUsedTime = object.m_lastUsedTime;
        m_modifyTime = object.m_modifyTime;
        m_objectType = object.m_objectType;
        m_resetTime = object.m_resetTime;
        m_size = object.m_size;
        m_startingJournalReceiverForApply = object.m_startingJournalReceiverForApply;
        m_startingJournalReceiverAsp = object.m_startingJournalReceiverAsp;
        m_hasChanged = object.m_hasChanged;
        m_applyJournalChangesRequired = object.m_applyJournalChangesRequired;
        m_checkedOut = object.m_checkedOut;
        m_journaled = object.m_journaled;
        m_pcArchive = object.m_pcArchive;
        m_pcHidden = object.m_pcHidden;
        m_pcReadOnly = object.m_pcReadOnly;
        m_pcSystem = object.m_pcSystem;
        m_saveAllowed = object.m_saveAllowed;
        m_storageFree = object.m_storageFree;
        m_storedLocally = object.m_storedLocally;
        m_journalsAfterImage = object.m_journalsAfterImage;
        m_journalsBeforeImage = object.m_journalsBeforeImage;
        m_journalsOptionalEntries = object.m_journalsOptionalEntries;
        m_name = object.m_name;
        m_as400 = object.m_as400;
        m_owner = object.m_owner;
        m_primaryGroup = object.m_primaryGroup;
        m_wasRollbackEnded = object.m_wasRollbackEnded;
    }
    return *this;
}

bool IfsObject::operator== (const IfsObject& object) const
{
    return m_as400 == object.m_as400 && m_name == object.m_name;
}

void IfsObject::ensureData()
{
    if (m_ccsid == -1)
        refresh();
    assert(m_ccsid != -1);
}

IfsObject::AttributeMap IfsObject::extractAttributes(const std::vector<uint8_t>& rawAttrs) const
{
    AttributeMap output;
    uint32_t offset = 0;
    uint32_t attrId;
    uint32_t length;
    do
    {
        memcpy(&attrId, &rawAttrs[offset + 4], 4);
        attrId = N32_TO_H(attrId);
        memcpy(&length, &rawAttrs[offset + 8], 4);
        length = N32_TO_H(length);
        if (length > 0)
        {
            output[attrId] = std::vector<uint8_t>(rawAttrs.begin() + offset + 16,
                                                  rawAttrs.begin() + offset + 16 + length);
        }
        memcpy(&offset, &rawAttrs[offset], 4);
        offset = N32_TO_H(offset);
    } while (offset != 0);
    return output;
}

FileClient& IfsObject::getFileClient()
{
    if (m_fileClient == 0)
        m_fileClient = new FileClient(*m_as400);
    return *m_fileClient;
}

void IfsObject::refresh()
{
    resetAttributes();
    std::pair<std::vector<uint8_t>, int> attrs = retrieveAttributes();
    AttributeMap attrMap = extractAttributes(attrs.first);
    updateAttributes(attrMap, attrs.second);
}

void IfsObject::reset()
{
    std::vector<uint8_t> data(2, 0);
    setAttribute(Qp0lAttr_RESET_DATE, data);
    refresh();
}

void IfsObject::resetAttributes()
{
    m_accessTime = 0;
    m_allocatedSize = 0;
    m_auditMode = AuditMode_NotAvailable;
    m_authorizationList = Text();
    m_asp = 0;
    m_ccsid = -1;
    m_changeTime = 0;
    m_checkedOutTime = 0;
    m_checkedOutUser = Text();
    m_codePage = 0;
    m_createTime = 0;
    m_daysUsed = 0;
    m_extendedAttributeSize = 0;
    m_fileId = std::vector<uint8_t>();
    m_journalId = std::vector<uint8_t>();
    m_journalName = QsysObjectName();
    m_journalStartTime = 0;
    m_lastUsedTime = 0;
    m_modifyTime = 0;
    m_objectType = Text();
    m_resetTime = 0;
    m_size = 0;
    m_startingJournalReceiverForApply = QsysObjectName();
    m_startingJournalReceiverAsp = Text();
    m_hasChanged = false;
    m_applyJournalChangesRequired = false;
    m_checkedOut = false;
    m_journaled = false;
    m_pcArchive = false;
    m_pcHidden = false;
    m_pcReadOnly = false;
    m_pcSystem = false;
    m_saveAllowed = false;
    m_storageFree = false;
    m_storedLocally = false;
    m_journalsAfterImage = false;
    m_journalsBeforeImage = false;
    m_journalsOptionalEntries = false;
    m_owner = Text();
    m_primaryGroup = Text();
    m_wasRollbackEnded = false;
}

std::pair<std::vector<uint8_t>, int> IfsObject::retrieveAttributes()
{
    size_t bufferProvided = 2048;
    ProgramParameterList parameters;
    parameters.addParameter(QlgPathName(m_name.getAbsolutePath(), ProgramParameter::PassBy_Reference));
    parameters.addParameter(InParameter(std::vector<uint8_t>(4, 0), ProgramParameter::PassBy_Reference));
    parameters.addParameter(OutParameter(bufferProvided, ProgramParameter::PassBy_Reference));
    parameters.addParameter(Int32InParameter(bufferProvided, ProgramParameter::PassBy_Value));
    parameters.addParameter(Int32OutParameter(ProgramParameter::PassBy_Reference));
    parameters.addParameter(Int32OutParameter(ProgramParameter::PassBy_Reference));
    parameters.addParameter(Int32InParameter(1, ProgramParameter::PassBy_Value));
    ServiceProgramProcedure getAttr(*m_as400,
                                    QsysObjectName(QSYS_LIB, QP0LLIB2_SRVPGM),
                                    QP0LGETATTR,
                                    parameters);
    getAttr.setReturnType(ServiceProgramProcedure::ReturnType_Integer);
    getAttr.setEmptyReturnIsSuccess(true);
    bool sizeTooSmall = false;
    do
    {
        if (!getAttr.run())
            throw InvalidStateException(__FILE__, __LINE__, "Failure running service program Qp0lGetAttr");
        if (getAttr.getReturnCode() != 0)
        {
            throw OS400ErrnoException(__FILE__, __LINE__,
                                      "Error running Qp0lGetAttr", getAttr.getErrorCode());
        }
        const Int32OutParameter& neededParam =
            dynamic_cast<const Int32OutParameter&>(getAttr.getParameters()[4]);
        if (bufferProvided < neededParam.getOutValue())
        {
            bufferProvided = neededParam.getOutValue();
            parameters[2] = OutParameter(bufferProvided, ProgramParameter::PassBy_Reference);
            parameters[3] = Int32InParameter(bufferProvided, ProgramParameter::PassBy_Value);
            getAttr.setParameters(parameters);
            sizeTooSmall = true;
        }
    }
    while (sizeTooSmall);
    return std::make_pair(dynamic_cast<const OutParameter&>(getAttr.getParameters()[2]).getOutData(),
                          getAttr.getCCSID());
}

void IfsObject::setAccessTime(Time time)
{
    uint32_t scratch = H32_TO_N(time / 1000);
    std::vector<uint8_t> data(4, 0);
    memcpy(&data[0], &scratch, 4);
    setAttribute(Qp0lAttr_ACCESS_TIME, data);
    m_accessTime = time;
}

void IfsObject::setAttribute(uint32_t attrId, const std::vector<uint8_t>& data)
{
    ProgramParameterList parameters;
    parameters.addParameter(QlgPathName(m_name.getAbsolutePath(), ProgramParameter::PassBy_Reference));
    parameters.addParameter(Qp0lAttrHeader(attrId, data, ProgramParameter::PassBy_Reference));
    parameters.addParameter(Int32InParameter(sizeof(Qp0l_Attr_Header_t) + data.size(), ProgramParameter::PassBy_Value));
    parameters.addParameter(Int32InParameter(1, ProgramParameter::PassBy_Value));
    ServiceProgramProcedure setAttr(*m_as400,
                                    QsysObjectName(QSYS_LIB, QP0LLIB3_SRVPGM),
                                    QP0LSETATTR,
                                    parameters);
    setAttr.setReturnType(ServiceProgramProcedure::ReturnType_Integer);
    setAttr.setEmptyReturnIsSuccess(true);
    if (!setAttr.run())
    {
        throw InvalidStateException(__FILE__, __LINE__, "Failure running service program Qp0lSetAttr");
    }
    else if (setAttr.getReturnCode() != 0)
    {
        throw OS400ErrnoException(__FILE__, __LINE__,
                                  "Error running Qp0lSetAttr", setAttr.getErrorCode());
    }
}

void IfsObject::setCCSID(int ccsid)
{
    uint32_t scratch = H32_TO_N(ccsid);
    std::vector<uint8_t> data(4, 0);
    memcpy(&data[0], &scratch, 4);
    setAttribute(Qp0lAttr_CCSID, data);
    m_ccsid = ccsid;
}

void IfsObject::setCodePage(int codePage)
{
    uint32_t scratch = H32_TO_N(codePage);
    std::vector<uint8_t> data(4, 0);
    memcpy(&data[0], &scratch, 4);
    setAttribute(Qp0lAttr_CCSID, data);
    m_codePage = codePage;
}

void IfsObject::setCreateTime(Time time)
{
    uint32_t scratch = H32_TO_N(time / 1000);
    std::vector<uint8_t> data(4, 0);
    memcpy(&data[0], &scratch, 4);
    setAttribute(Qp0lAttr_CREATE_TIME, data);
    m_createTime = time;
}

void IfsObject::setHasChanged(bool hasChanged)
{
    std::vector<uint8_t> data;
    data.push_back(hasChanged ? 1 : 0);
    setAttribute(Qp0lAttr_SYSTEM_ARCHIVE, data);
    m_hasChanged = hasChanged;
}

void IfsObject::setSaveAllowed(bool saveAllowed)
{
    std::vector<uint8_t> data;
    data.push_back(saveAllowed ? 1 : 0);
    setAttribute(Qp0lAttr_ALWSAV, data);
    m_saveAllowed = saveAllowed;
}

void IfsObject::setModifyTime(Time time)
{
    uint32_t scratch = H32_TO_N(time / 1000);
    std::vector<uint8_t> data(4, 0);
    memcpy(&data[0], &scratch, 4);
    setAttribute(Qp0lAttr_MODIFY_TIME, data);
    m_modifyTime = time;
}

void IfsObject::setPCArchive(bool archive)
{
    std::vector<uint8_t> data;
    data.push_back(archive ? 1 : 0);
    setAttribute(Qp0lAttr_PC_ARCHIVE, data);
    m_pcArchive = archive;
}

void IfsObject::setPCHidden(bool hidden)
{
    std::vector<uint8_t> data;
    data.push_back(hidden ? 1 : 0);
    setAttribute(Qp0lAttr_PC_HIDDEN, data);
    m_pcHidden = hidden;
}

void IfsObject::setPCReadOnly(bool readOnly)
{
    std::vector<uint8_t> data;
    data.push_back(readOnly ? 1 : 0);
    setAttribute(Qp0lAttr_PC_READ_ONLY, data);
    m_pcReadOnly = readOnly;
}

void IfsObject::setPCSystem(bool system)
{
    std::vector<uint8_t> data;
    data.push_back(system ? 1 : 0);
    setAttribute(Qp0lAttr_PC_SYSTEM, data);
    m_pcSystem = system;
}

void IfsObject::updateAttributes(const AttributeMap& attrMap, int ccsid)
{
    static Qp0lAttr myAttrs[] =
    {
        Qp0lAttr_ACCESS_TIME, Qp0lAttr_ALLOC_SIZE_64, Qp0lAttr_AUDIT, Qp0lAttr_AUTH,
        Qp0lAttr_ASP, Qp0lAttr_CCSID, Qp0lAttr_CHANGE_TIME, Qp0lAttr_CHECKED_OUT,
        Qp0lAttr_CODEPAGE, Qp0lAttr_CREATE_TIME, Qp0lAttr_USAGE_INFORMATION,
        Qp0lAttr_EXTENDED_ATTR_SIZE, Qp0lAttr_FILE_ID, Qp0lAttr_JOURNAL_EXTENDED_INFORMATION,
        Qp0lAttr_MODIFY_TIME, Qp0lAttr_OBJTYPE, Qp0lAttr_DATA_SIZE_64, Qp0lAttr_SYSTEM_ARCHIVE,
        Qp0lAttr_PC_ARCHIVE, Qp0lAttr_PC_HIDDEN, Qp0lAttr_PC_READ_ONLY, Qp0lAttr_PC_SYSTEM,
        Qp0lAttr_ALWSAV, Qp0lAttr_STG_FREE, Qp0lAttr_LOCAL_REMOTE
    };
    static size_t myAttrCount = sizeof(myAttrs) / sizeof(Qp0lAttr);

    uint32_t scratch32;
    uint64_t scratch64;
    for (unsigned i = 0; i < myAttrCount; i++)
    {
        AttributeMap::const_iterator found = attrMap.find(myAttrs[i]);
        if (found == attrMap.end())
            continue;
        const std::vector<uint8_t>& attr = found->second;
        if (myAttrs[i] == Qp0lAttr_ACCESS_TIME)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_accessTime = makeTime(N32_TO_H(scratch32));
        }
        else if (myAttrs[i] == Qp0lAttr_ALLOC_SIZE_64)
        {
            assert(attr.size() == 8);
            memcpy(&scratch64, &attr[0], 8);
            m_allocatedSize = N64_TO_H(scratch64);
        }
        else if (myAttrs[i] == Qp0lAttr_AUDIT)
        {
            m_auditMode = AttributeUtility::getAuditMode(&attr[0], ccsid);
        }
        else if (myAttrs[i] == Qp0lAttr_AUTH)
        {
            assert(attr.size() >= 30);
            m_owner = Text(&attr[0], 10, ccsid);
            m_owner.trim();
            m_primaryGroup = Text(&attr[10], 10, ccsid);
            m_primaryGroup.trim();
            m_authorizationList = Text(&attr[20], 10, ccsid);
            m_authorizationList.trim();
        }
        else if (myAttrs[i] == Qp0lAttr_ASP)
        {
            assert(attr.size() == 2);
            memcpy(&m_asp, &attr[0], 2);
            m_asp = N16_TO_H(m_asp);
        }
        else if (myAttrs[i] == Qp0lAttr_CCSID)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_ccsid = N32_TO_H(scratch32);
        }
        else if (myAttrs[i] == Qp0lAttr_CHANGE_TIME)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_changeTime = makeTime(N32_TO_H(scratch32));
        }
        else if (myAttrs[i] == Qp0lAttr_CHECKED_OUT)
        {
            assert(attr.size() == 16);
            m_checkedOut = attr[0] != 0;
            m_checkedOutUser = Text(&attr[1], 10, ccsid);
            m_checkedOutUser.trim();
            memcpy(&scratch32, &attr[12], 4);
            m_checkedOutTime = makeTime(N32_TO_H(scratch32));
        }
        else if (myAttrs[i] == Qp0lAttr_CODEPAGE)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_codePage = N32_TO_H(scratch32);
        }
        else if (myAttrs[i] == Qp0lAttr_CREATE_TIME)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_createTime = makeTime(N32_TO_H(scratch32));
        }
        else if (myAttrs[i] == Qp0lAttr_USAGE_INFORMATION)
        {
            assert(attr.size() >= 12);
            memcpy(&scratch32, &attr[0], 4);
            m_resetTime = makeTime(N32_TO_H(scratch32));
            memcpy(&scratch32, &attr[4], 4);
            m_lastUsedTime = makeTime(N32_TO_H(scratch32));
            memcpy(&scratch32, &attr[8], 4);
            m_daysUsed = N32_TO_H(scratch32);
        }
        else if (myAttrs[i] == Qp0lAttr_EXTENDED_ATTR_SIZE)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_extendedAttributeSize = N32_TO_H(scratch32);
        }
        else if (myAttrs[i] == Qp0lAttr_FILE_ID)
        {
            assert(attr.size() == 16);
            m_fileId = attr;
        }
        else if (myAttrs[i] == Qp0lAttr_JOURNAL_EXTENDED_INFORMATION)
        {
            assert(attr.size() >= 68);
            m_journaled = attr[0] != 0;
            m_journalsOptionalEntries = (attr[1] & 0x08) != 0;
            m_journalsAfterImage = (attr[1] & 0x20) != 0;
            m_journalsBeforeImage = (attr[1] & 0x40) != 0;
            m_journalId.assign(attr.begin() + 2, attr.begin() + 12);
            Text libName(&attr[22], 10, ccsid);
            Text journalName(&attr[12], 10, ccsid);
            m_journalName = QsysObjectName(libName, journalName);
            libName = Text(&attr[46], 10, ccsid);
            Text receiverName(&attr[36], 10, ccsid);
            m_startingJournalReceiverForApply = QsysObjectName(libName, receiverName);
            m_startingJournalReceiverAsp = Text(&attr[56], 10, ccsid);
            m_startingJournalReceiverAsp.trim();
            memcpy(&scratch32, &attr[32], 4);
            m_journalStartTime = makeTime(N32_TO_H(scratch32));
            m_applyJournalChangesRequired = attr[66] != 0;
            m_wasRollbackEnded = attr[67] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_MODIFY_TIME)
        {
            assert(attr.size() == 4);
            memcpy(&scratch32, &attr[0], 4);
            m_modifyTime = makeTime(N32_TO_H(scratch32));
        }
        else if (myAttrs[i] == Qp0lAttr_OBJTYPE)
        {
            assert(attr.size() > 0);
            m_objectType = Text(attr, ccsid);
            m_objectType.trim();
        }
        else if (myAttrs[i] == Qp0lAttr_DATA_SIZE_64)
        {
            assert(attr.size() == 8);
            memcpy(&scratch64, &attr[0], 8);
            m_size = N64_TO_H(scratch64);
        }
        else if (myAttrs[i] == Qp0lAttr_SYSTEM_ARCHIVE)
        {
            assert(attr.size() == 1);
            m_hasChanged = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_PC_ARCHIVE)
        {
            assert(attr.size() == 1);
            m_pcArchive = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_PC_HIDDEN)
        {
            assert(attr.size() == 1);
            m_pcHidden = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_PC_READ_ONLY)
        {
            assert(attr.size() == 1);
            m_pcReadOnly = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_PC_SYSTEM)
        {
            assert(attr.size() == 1);
            m_pcSystem = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_ALWSAV)
        {
            assert(attr.size() == 1);
            m_saveAllowed = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_STG_FREE)
        {
            assert(attr.size() == 1);
            m_storageFree = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_LOCAL_REMOTE)
        {
            assert(attr.size() == 1);
            m_storedLocally = attr[0] == 1;
        }
    }
}

}
