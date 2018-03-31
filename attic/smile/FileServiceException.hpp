#if !defined(SMILE_FILESERVICEEXCEPTION_HPP__)
#define SMILE_FILESERVICEEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>
#include <stdint.h>

namespace smile
{

class FileServiceException : public Exception
{
public:
    enum ErrorCode
    {
        ErrorCode_NoError                       = 0,
        ErrorCode_FileInUse                     = 1,
        ErrorCode_FileNotFound                  = 2,
        ErrorCode_PathNotFound                  = 3,
        ErrorCode_DuplicateDirEntryName         = 4,
        ErrorCode_AccessDeniedToDirEntry        = 5,
        ErrorCode_InvalidHandle                 = 6,
        ErrorCode_InvalidDirEntryName           = 7,
        ErrorCode_InvalidAttributeName          = 8,
        ErrorCode_DirIsNotEmpty                 = 9,
        ErrorCode_FileSubstreamInUse            = 10,
        ErrorCode_ResourceLimitExceeded         = 11,
        ErrorCode_ResourceNotAvailable          = 12,
        ErrorCode_AccessDeniedToRequest         = 13,
        ErrorCode_DirEntryDamaged               = 14,
        ErrorCode_InvalidConnection             = 15,
        ErrorCode_InvalidRequest                = 16,
        ErrorCode_DataStreamSyntaxError         = 17,
        ErrorCode_NoMoreFiles                   = 18,
        ErrorCode_ParameterNotSupported         = 19,
        ErrorCode_ParameterValueNotSupported    = 20,
        ErrorCode_CannotConvertValue            = 21,
        ErrorCode_NoMoreData                    = 22,
        ErrorCode_RequestNotSupported           = 23,
        ErrorCode_InvalidUser                   = 24,
        ErrorCode_UnknownError                  = 25,
        ErrorCode_SharingViolation              = 32,
        ErrorCode_LockViolation                 = 33,
        ErrorCode_UnknownToSmile                = -1
    };

    static ErrorCode convertErrorCode(uint32_t rawErrorCode);

    FileServiceException(const char* const fileName,
                         int lineNumber,
                         const char* const msg,
                         uint32_t rawErrorCode);
    FileServiceException(const FileServiceException& except);

    FileServiceException& operator= (const FileServiceException& except);

    virtual Exception* duplicate() const;
    ErrorCode getErrorCode() const;
    uint32_t getRawErrorCode() const;

private:
    const char* getErrorCodeMessage(ErrorCode errorCode) const;

    uint32_t m_rawErrorCode;
};

inline FileServiceException::ErrorCode FileServiceException::getErrorCode() const
{
    return convertErrorCode(m_rawErrorCode);
}

inline uint32_t FileServiceException::getRawErrorCode() const
{
    return m_rawErrorCode;
}

}

#endif
