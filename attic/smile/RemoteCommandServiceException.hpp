#if !defined(SMILE_REMOTECOMMANDSERVICEEXCEPTION_HPP__)
#define SMILE_REMOTECOMMANDSERVICEEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>
#include <stdint.h>

namespace smile
{

class RemoteCommandServiceException : public Exception
{
public:
    enum ErrorCode
    {
        ErrorCode_None                                  = 0x0000,
        ErrorCode_LimitedUser                           = 0x0100,
        ErrorCode_InvalidExchangeAttributesRequest      = 0x0101,
        ErrorCode_InvalidDataStreamLevel                = 0x0102,
        ErrorCode_InvalidVersion                        = 0x0103,
        ErrorCode_InvalidCCSID                          = 0x0104,
        ErrorCode_InvalidNationalLanguageVersion        = 0x0105,
        ErrorCode_NationalLanguageVersionNotInstalled   = 0x0106,
        ErrorCode_CantValidateNationalLanguageVersion   = 0x0107,
        ErrorCode_CantAddNLVLibraryToSystemLibraryList  = 0x0108,
        ErrorCode_UnableToProcessRequest                = 0x0200,
        ErrorCode_InvalidLL                             = 0x0201,
        ErrorCode_InvalidServerId                       = 0x0202,
        ErrorCode_IncompleteData                        = 0x0203,
        ErrorCode_HostResourceError                     = 0x0204,
        ErrorCode_InvalidRequestId                      = 0x0205,
        ErrorCode_ProcessExitPoint                      = 0x0300,
        ErrorCode_InvalidRequest                        = 0x0301,
        ErrorCode_InvalidParameter                      = 0x0302,
        ErrorCode_MaximumExceeded                       = 0x0303,
        ErrorCode_CallingUserExitProgram                = 0x0304,
        ErrorCode_UserExitProgramDeniedRequest          = 0x0305,
        ErrorCode_CommandFailed                         = 0x0400,
        ErrorCode_InvalidCCSID_2                        = 0x0401,
        ErrorCode_ResolvingProgram                      = 0x0500,
        ErrorCode_CallingProgram                        = 0x0501,
        ErrorCode_Unknown                               = 0xffff
    };

    RemoteCommandServiceException(const char* const fileName,
                                  int lineNumber,
                                  const char* const msg,
                                  uint16_t rawErrorCode);
    RemoteCommandServiceException(const RemoteCommandServiceException& except);

    RemoteCommandServiceException& operator= (const RemoteCommandServiceException& except);

    virtual Exception* duplicate() const;
    ErrorCode getErrorCode() const;
    uint16_t getRawErrorCode() const;

private:
    ErrorCode convertErrorCode(uint32_t rawErrorCode) const;
    const char* getErrorCodeMessage(ErrorCode errorCode) const;

    uint16_t m_rawErrorCode;
};

inline RemoteCommandServiceException::ErrorCode RemoteCommandServiceException::getErrorCode() const
{
    return convertErrorCode(m_rawErrorCode);
}

inline uint16_t RemoteCommandServiceException::getRawErrorCode() const
{
    return m_rawErrorCode;
}

}

#endif
