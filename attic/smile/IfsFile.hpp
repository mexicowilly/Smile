#if !defined(SMILE_IFSFILE_HPP__)
#define SMILE_IFSFILE_HPP__

#include <smile/IfsObject.hpp>

namespace smile
{

class IfsFile : public IfsObject
{
public:
    enum FileFormat
    {
        FileFormat_Type1 = 0,
        FileFormat_Type2 = 1
    };

    enum ScanStatus
    {
        ScanStatus_Required     = 0,
        ScanStatus_Success      = 1,
        ScanStatus_Failure      = 2,
        ScanStatus_PendingCVN   = 5,
        ScanStatus_NotRequired  = 6
    };

    enum StorageOption
    {
        StorageOption_Normal    = 0,
        StorageOption_Minimize  = 1,
        StorageOption_Dynamic   = 2
    };

    enum SystemUse
    {
        SystemUse_None                      = 0,
        SystemUse_VirtualVolume             = 1,
        SystemUse_NetworkServerStorageSpace = 2
    };

    enum UdfsDefaultFormat
    {
        UdfsDefaultFormat_Type1 = 0,
        UdfsDefaultFormat_Type2 = 1
    };

    IfsFile(AS400& as400, const IfsObjectName& name);
    IfsFile(const IfsFile& file);

    IfsFile& operator= (const IfsFile& file);

    virtual IfsObject* duplicate() const;
    virtual bool exists();
    StorageOption getAuxiliaryStorageOption();
    FileFormat getFileFormat();
    StorageOption getMainStorageOption();
    ScanMode getScanMode();
    ScanStatus getScanStatus();
    SystemUse getSystemUse();
    UdfsDefaultFormat getUdfsDefaultFormat();
    bool hasMultipleSignatures();
    bool isScanSignatureDifferent();
    bool isSetGID();
    bool isSetUID();
    bool isSharedWithWritersDuringCheckpointWrite();
    bool isSigned();
    bool isSystemSigned();
    virtual void remove();
    void setAuxiliaryStorageOption(StorageOption storageOption);
    void setMainStorageOption(StorageOption storageOption);
    void setScanMode(ScanMode scanMode);
    void setSetGID(bool setGid);
    void setSetUID(bool setUid);
    void setSharedWithWritersDuringCheckpointWrite(bool newState);
    bool wasScannedInBinaryMode();

protected:
    virtual void resetAttributes();
    virtual void updateAttributes(const AttributeMap& attrMap, int ccsid);

private:
    StorageOption m_auxiliaryStorageOption;
    FileFormat m_fileFormat;
    StorageOption m_mainStorageOption;
    ScanMode m_scanMode;
    ScanStatus m_scanStatus;
    SystemUse m_systemUse;
    UdfsDefaultFormat m_udfsDefaultFormat;
    bool m_multipleSignatures;
    bool m_scanSignatureDifferent;
    bool m_setGid;
    bool m_setUid;
    bool m_sharedWithWritersDuringCheckpointWrite;
    bool m_signed;
    bool m_systemSigned;
    bool m_scannedInBinaryMode;
};

inline IfsFile::StorageOption IfsFile::getAuxiliaryStorageOption()
{
    ensureData();
    return m_auxiliaryStorageOption;
}

inline IfsFile::FileFormat IfsFile::getFileFormat()
{
    ensureData();
    return m_fileFormat;
}

inline IfsFile::StorageOption IfsFile::getMainStorageOption()
{
    ensureData();
    return m_mainStorageOption;
}

inline IfsObject::ScanMode IfsFile::getScanMode()
{
    ensureData();
    return m_scanMode;
}

inline IfsFile::ScanStatus IfsFile::getScanStatus()
{
   ensureData();
   return m_scanStatus;
}

inline IfsFile::SystemUse IfsFile::getSystemUse()
{
    ensureData();
    return m_systemUse;
}

inline IfsFile::UdfsDefaultFormat IfsFile::getUdfsDefaultFormat()
{
    ensureData();
    return m_udfsDefaultFormat;
}

inline bool IfsFile::hasMultipleSignatures()
{
    ensureData();
    return m_multipleSignatures;
}

inline bool IfsFile::isScanSignatureDifferent()
{
    ensureData();
    return m_scanSignatureDifferent;
}

inline bool IfsFile::isSetGID()
{
    ensureData();
    return m_setGid;
}

inline bool IfsFile::isSetUID()
{
    ensureData();
    return m_setUid;
}

inline bool IfsFile::isSharedWithWritersDuringCheckpointWrite()
{
    ensureData();
    return m_sharedWithWritersDuringCheckpointWrite;
}
inline bool IfsFile::isSigned()
{
    ensureData();
    return m_signed;
}

inline bool IfsFile::isSystemSigned()
{
    ensureData();
    return m_systemSigned;
}

inline bool IfsFile::wasScannedInBinaryMode()
{
    ensureData();
    return m_scannedInBinaryMode;
}

}

#endif
