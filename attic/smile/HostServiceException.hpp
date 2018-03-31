#if !defined(SMILE_HOSTSERVICEEXCEPTION_HPP__)
#define SMILE_HOSTSERVICEEXCEPTION_HPP__

#include <smile/ExceptionBase.hpp>
#include <stdint.h>

namespace smile
{

class HostServiceException : public Exception
{
public:
    enum ErrorCode
    {
        ErrorCode_None                                      = 0x00000000,
        ErrorCode_InvalidRandomSeedExchange                 = 0x00010001,
        ErrorCode_InvalidServerId                           = 0x00010002,
        ErrorCode_InvalidRequestId                          = 0x00010003,
        ErrorCode_InvalidRandomSeed                         = 0x00010004,
        ErrorCode_RandomSeedMissing                         = 0x00010005,
        ErrorCode_InvalidPasswordEncryptIndicator           = 0x00010006,
        ErrorCode_InvalidUserIdLength                       = 0x00010007,
        ErrorCode_InvalidPasswordLength                     = 0x00010008,
        ErrorCode_InvalidClientVersionOrSendReplyIndicator  = 0x00010009,
        ErrorCode_InvalidRequestData                        = 0x0001000a,
        ErrorCode_InvalidSignonRequest                      = 0x0001000b,
        ErrorCode_InvalidChangePasswordRequest              = 0x0001000c,
        ErrorCode_InvalidOldPassword                        = 0x0001000d,
        ErrorCode_InvalidNewPassword                        = 0x0001000e,
        ErrorCode_InvalidTokenType                          = 0x0001000f,
        ErrorCode_InvalidGenerateTokenRequest               = 0x00010010,
        ErrorCode_InvalidTokenLength                        = 0x00010011,
        ErrorCode_InvalidGenerateTokenForOtherUserRequest   = 0x00010012,
        ErrorCode_UnknownUserId                             = 0x00020001,
        ErrorCode_UserIdValidButRevoked                     = 0x00020002,
        ErrorCode_UserIdMismatch                            = 0x00020003,
        ErrorCode_NewPasswordTooLong                        = 0x00030001,
        ErrorCode_NewPasswordTooShort                       = 0x00030002,
        ErrorCode_NewPasswordRepeatCharacter                = 0x00030003,
        ErrorCode_NewPasswordAdjacentDigits                 = 0x00030004,
        ErrorCode_NewPasswordCharacterConsecutiveRepeat     = 0x00030005,
        ErrorCode_NewPasswordPreviouslyUsed                 = 0x00030006,
        ErrorCode_NewPasswordMissingNumeral                 = 0x00030007,
        ErrorCode_NewPasswordInvalidCharacter               = 0x00030008,
        ErrorCode_NewPasswordDisallowed                     = 0x00030009,
        ErrorCode_NewPasswordContainsUserId                 = 0x0003000a,
        ErrorCode_PasswordIncorrect                         = 0x0003000b,
        ErrorCode_UserIdDisabledOnNextFailedSignon          = 0x0003000c,
        ErrorCode_PasswordCorrectButExpired                 = 0x0003000d,
        ErrorCode_PasswordPreV2R2Encryption                 = 0x0003000e,
        ErrorCode_NewPasswordCharacterInSamePositionAsOld   = 0x0003000f,
        ErrorCode_PasswordIsStarNone                        = 0x00030010,
        ErrorCode_PasswordValidationFailed                  = 0x00030011,
        ErrorCode_PasswordChangeNotAllowed                  = 0x00030012,
        ErrorCode_InvalidPasswordValue                      = 0x00030013,
        ErrorCode_GeneralSecurity                           = 0x00040000,
        ErrorCode_IncorrectProgramDataLength                = 0x00040001,
        ErrorCode_ServerJobTimeout                          = 0x00040002,
        ErrorCode_ServerJobNotStarted                       = 0x00040003,
        ErrorCode_PrestartJobNotStarted                     = 0x00040004,
        ErrorCode_SubsystemFailure                          = 0x00040005,
        ErrorCode_ServerJobEnding                           = 0x00040006,
        ErrorCode_ReceiverAreaTooSmall                      = 0x00040007,
        ErrorCode_ConnectionNotPassed                       = 0x00040008,
        ErrorCode_NonexistentServerJobUserId                = 0x00040009,
        ErrorCode_ServerJobAuthorityFailure                 = 0x0004000a,
        ErrorCode_ServerJobProgramNotFound                  = 0x0004000b,
        ErrorCode_DaemonJobServerJobLibraryAuthority        = 0x0004000c,
        ErrorCode_DaemonJobServerJobProgramAuthority        = 0x0004000d,
        ErrorCode_UserNotAuthorizedToGenerateToken          = 0x0004000e,
        ErrorCode_NoMemoryToGenerateToken                   = 0x0004000f,
        ErrorCode_TextConversion                            = 0x00040010,
        ErrorCode_EimInterface                              = 0x00040011,
        ErrorCode_CryptoInterface                           = 0x00040012,
        ErrorCode_UnsupportedTokenVersion                   = 0x00040013,
        ErrorCode_PublicKeyNotFound                         = 0x00040014,
        ErrorCode_ExitPointProcessingFailure                = 0x00050001,
        ErrorCode_CouldNotResolveExitProgram                = 0x00050002,
        ErrorCode_CouldNotCallExitProgram                   = 0x00050003,
        ErrorCode_ExitProgramDeniedRequest                  = 0x00050004,
        ErrorCode_InvalidToken                              = 0x00060001,
        ErrorCode_TooManyTokens                             = 0x00060002,
        ErrorCode_InvalidTimeoutInterval                    = 0x00060003,
        ErrorCode_InvalidTokenRequest                       = 0x00060004,
        ErrorCode_TokenNotRegenerable                       = 0x00060005,
        ErrorCode_KerberosConsistencyCheckFailure           = 0x00060006,
        ErrorCode_AuthenticationMechanismsNotSupported      = 0x00060007,
        ErrorCode_InvalidCredentials                        = 0x00060008,
        ErrorCode_InvalidKerberosTicketSignature            = 0x00060009,
        ErrorCode_InvalidKerberosCredentials                = 0x0006000a,
        ErrorCode_InconsistentKerberosTicket                = 0x0006000b,
        ErrorCode_VerificationFailure                       = 0x0006000c,
        ErrorCode_EimConfiguration                          = 0x0006000d,
        ErrorCode_InvalidKerberosSystemProfile              = 0x0006000e,
        ErrorCode_PrincipalMapsToMultipleUserIds            = 0x0006000f,
        ErrorCode_CannotConnectToEimDomain                  = 0x00070001,
        ErrorCode_CannotChangeCCSIDForEimRequests           = 0x00070002,
        ErrorCode_CannotObtainEimRegistryName               = 0x00070003,
        ErrorCode_NoWebsphereToOs400ProfileMappingExists    = 0x00070004,
        ErrorCode_Unknown                                   = 0xffffffff
    };

    HostServiceException(const char* const fileName,
                         int lineNumber,
                         const char* const msg,
                         uint32_t rawErrorCode);
    HostServiceException(const HostServiceException& except);

    HostServiceException& operator= (const HostServiceException& except);

    virtual Exception* duplicate() const;
    ErrorCode getErrorCode() const;
    uint32_t getRawErrorCode() const;

private:
    ErrorCode convertErrorCode(uint32_t rawErrorCode) const;
    const char* getErrorCodeMessage(ErrorCode errorCode) const;

    uint32_t m_rawErrorCode;
};

inline HostServiceException::ErrorCode HostServiceException::getErrorCode() const
{
    return convertErrorCode(m_rawErrorCode);
}

inline uint32_t HostServiceException::getRawErrorCode() const
{
    return m_rawErrorCode;
}

}

#endif
