#include "smile/IfsFile.hpp"
#include "IfsAttributes.hpp"
#include "FileClient.hpp"
#include <assert.h>

namespace smile
{

IfsFile::IfsFile(AS400& as400, const IfsObjectName& name)
    : IfsObject(as400, name),
      m_auxiliaryStorageOption(StorageOption_Normal),
      m_fileFormat(FileFormat_Type1),
      m_mainStorageOption(StorageOption_Normal),
      m_scanMode(ScanMode_No),
      m_scanStatus(ScanStatus_NotRequired),
      m_systemUse(SystemUse_None),
      m_udfsDefaultFormat(UdfsDefaultFormat_Type1),
      m_multipleSignatures(false),
      m_scanSignatureDifferent(false),
      m_setGid(false),
      m_setUid(false),
      m_sharedWithWritersDuringCheckpointWrite(false),
      m_signed(false),
      m_systemSigned(false),
      m_scannedInBinaryMode(false)
{
}

IfsFile::IfsFile(const IfsFile& file)
    : IfsObject(file),
      m_auxiliaryStorageOption(file.m_auxiliaryStorageOption),
      m_fileFormat(file.m_fileFormat),
      m_mainStorageOption(file.m_mainStorageOption),
      m_scanMode(file.m_scanMode),
      m_scanStatus(file.m_scanStatus),
      m_systemUse(file.m_systemUse),
      m_udfsDefaultFormat(file.m_udfsDefaultFormat),
      m_multipleSignatures(file.m_multipleSignatures),
      m_scanSignatureDifferent(file.m_scanSignatureDifferent),
      m_setGid(file.m_setGid),
      m_setUid(file.m_setUid),
      m_sharedWithWritersDuringCheckpointWrite(file.m_sharedWithWritersDuringCheckpointWrite),
      m_signed(file.m_signed),
      m_systemSigned(file.m_systemSigned),
      m_scannedInBinaryMode(file.m_scannedInBinaryMode)
{
}

IfsFile& IfsFile::operator= (const IfsFile& file)
{
    if (&file != this)
    {
        IfsObject::operator=(file);
        m_auxiliaryStorageOption = file.m_auxiliaryStorageOption;
        m_fileFormat = file.m_fileFormat;
        m_mainStorageOption = file.m_mainStorageOption;
        m_scanMode = file.m_scanMode;
        m_scanStatus = file.m_scanStatus;
        m_systemUse = file.m_systemUse;
        m_udfsDefaultFormat = file.m_udfsDefaultFormat;
        m_multipleSignatures = file.m_multipleSignatures;
        m_scanSignatureDifferent = file.m_scanSignatureDifferent;
        m_setGid = file.m_setGid;
        m_setUid = file.m_setUid;
        m_sharedWithWritersDuringCheckpointWrite = file.m_sharedWithWritersDuringCheckpointWrite;
        m_signed = file.m_signed;
        m_systemSigned = file.m_systemSigned;
        m_scannedInBinaryMode = file.m_scannedInBinaryMode;
    }
    return *this;
}

IfsObject* IfsFile::duplicate() const
{
    return new IfsFile(*this);
}

bool IfsFile::exists()
{
    return getFileClient().exists(getName().getAbsolutePath()) ==
        FileClient::Existence_File;
}

void IfsFile::remove()
{
    getFileClient().deleteFile(getName().getAbsolutePath());
}

void IfsFile::resetAttributes()
{
    IfsObject::resetAttributes();
    m_auxiliaryStorageOption = StorageOption_Normal;
    m_fileFormat = FileFormat_Type1;
    m_mainStorageOption = StorageOption_Normal;
    m_scanMode = ScanMode_No;
    m_scanStatus = ScanStatus_NotRequired;
    m_systemUse = SystemUse_None;
    m_udfsDefaultFormat = UdfsDefaultFormat_Type1;
    m_multipleSignatures = false;
    m_scanSignatureDifferent = false;
    m_setGid = false;
    m_setUid = false;
    m_sharedWithWritersDuringCheckpointWrite = false;
    m_signed = false;
    m_systemSigned = false;
    m_scannedInBinaryMode = false;
}

void IfsFile::setAuxiliaryStorageOption(StorageOption storageOption)
{
    std::vector<uint8_t> data;
    data.push_back(storageOption);
    setAttribute(Qp0lAttr_DISK_STG_OPT, data);
    m_auxiliaryStorageOption = storageOption;
}

void IfsFile::setMainStorageOption(StorageOption storageOption)
{
    std::vector<uint8_t> data;
    data.push_back(storageOption);
    setAttribute(Qp0lAttr_MAIN_STG_OPT, data);
    m_mainStorageOption = storageOption;
}

void IfsFile::setScanMode(ScanMode scanMode)
{
    std::vector<uint8_t> data;
    data.push_back(scanMode);
    setAttribute(Qp0lAttr_SCAN, data);
    m_scanMode = scanMode;
}

void IfsFile::setSetGID(bool setGid)
{
    std::vector<uint8_t> data;
    data.push_back(setGid ? 1 : 0);
    setAttribute(Qp0lAttr_SGID, data);
    m_setGid = setGid;
}

void IfsFile::setSetUID(bool setUid)
{
    std::vector<uint8_t> data;
    data.push_back(setUid ? 1 : 0);
    setAttribute(Qp0lAttr_SUID, data);
    m_setUid = setUid;
}

void IfsFile::setSharedWithWritersDuringCheckpointWrite(bool newState)
{
    std::vector<uint8_t> data;
    data.push_back(newState ? 1 : 0);
    setAttribute(Qp0lAttr_ALWCKPWRT, data);
    m_sharedWithWritersDuringCheckpointWrite = newState;
}

void IfsFile::updateAttributes(const AttributeMap& attrMap, int ccsid)
{
    static Qp0lAttr myAttrs[] =
    {
        Qp0lAttr_DISK_STG_OPT, Qp0lAttr_FILE_FORMAT, Qp0lAttr_MAIN_STG_OPT,
        Qp0lAttr_SCAN, Qp0lAttr_SCAN_INFO, Qp0lAttr_SYSTEM_USE,
        Qp0lAttr_UDFS_DEFAULT_FORMAT, Qp0lAttr_MULTI_SIGS, Qp0lAttr_SGID,
        Qp0lAttr_SUID, Qp0lAttr_ALWCKPWRT, Qp0lAttr_SIGNED, Qp0lAttr_SYS_SIGNED
    };
    static size_t myAttrCount = sizeof(myAttrs) / sizeof(myAttrs[0]);

    IfsObject::updateAttributes(attrMap, ccsid);
    for (unsigned i = 0; i < myAttrCount; i++)
    {
        AttributeMap::const_iterator found = attrMap.find(myAttrs[i]);
        if (found == attrMap.end())
            continue;
        const std::vector<uint8_t>& attr = found->second;
        if (myAttrs[i] == Qp0lAttr_DISK_STG_OPT)
        {
            assert(attr.size() == 1);
            m_auxiliaryStorageOption = static_cast<StorageOption>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_FILE_FORMAT)
        {
            assert(attr.size() == 1);
            m_fileFormat = static_cast<FileFormat>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_MAIN_STG_OPT)
        {
            assert(attr.size() == 1);
            m_mainStorageOption = static_cast<StorageOption>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_SCAN)
        {
            assert(attr.size() == 1);
            m_scanMode = static_cast<ScanMode>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_SCAN_INFO)
        {
            assert(attr.size() >= 3);
            m_scanStatus = static_cast<ScanStatus>(attr[0]);
            m_scanSignatureDifferent = attr[1] != 0;
            m_scannedInBinaryMode = attr[2] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_SYSTEM_USE)
        {
            assert(attr.size() == 1);
            m_systemUse = static_cast<SystemUse>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_UDFS_DEFAULT_FORMAT)
        {
            assert(attr.size() == 1);
            m_udfsDefaultFormat = static_cast<UdfsDefaultFormat>(attr[0]);
        }
        else if (myAttrs[i] == Qp0lAttr_MULTI_SIGS)
        {
            assert(attr.size() == 1);
            m_multipleSignatures = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_SGID)
        {
            assert(attr.size() == 1);
            m_setGid = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_SUID)
        {
            assert(attr.size() == 1);
            m_setUid = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_ALWCKPWRT)
        {
            assert(attr.size() == 1);
            m_sharedWithWritersDuringCheckpointWrite = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_SIGNED)
        {
            assert(attr.size() == 1);
            m_signed = attr[0] != 0;
        }
        else if (myAttrs[i] == Qp0lAttr_SYS_SIGNED)
        {
            assert(attr.size() == 1);
            m_systemSigned = attr[0] != 0;
        }
    }
}

}
