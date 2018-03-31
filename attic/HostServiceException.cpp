#include <smile/HostServiceException.hpp>
#include <sstream>
#include <algorithm>

namespace smile
{

HostServiceException::HostServiceException(const char* const fileName,
                                           int lineNumber,
                                           const char* const msg,
                                           uint32_t rawErrorCode)
    : Exception(fileName, lineNumber), m_rawErrorCode(rawErrorCode)
{
    ErrorCode code = convertErrorCode(rawErrorCode);
    std::ostringstream stream;
    stream << msg << ": " << getErrorCodeMessage(code);
    if (code == ErrorCode_Unknown)
        stream << " (" << std::hex << std::showbase << m_rawErrorCode << ")";
    m_what = new char[stream.str().length() + 1];
    strcpy(m_what, stream.str().c_str());
}

HostServiceException::ErrorCode HostServiceException::convertErrorCode(uint32_t rawErrorCode) const
{
    static uint32_t errorCodes[] =
    {
        0x00000000, 0x00010001, 0x00010002, 0x00010003, 0x00010004, 0x00010005, 0x00010006,
        0x00010007, 0x00010008, 0x00010009, 0x0001000a, 0x0001000b, 0x0001000c, 0x0001000d,
        0x0001000e, 0x0001000f, 0x00010010, 0x00010011, 0x00010012, 0x00020001, 0x00020002,
        0x00020003, 0x00030001, 0x00030002, 0x00030003, 0x00030004, 0x00030005, 0x00030006,
        0x00030007, 0x00030008, 0x00030009, 0x0003000a, 0x0003000b, 0x0003000c, 0x0003000d,
        0x0003000e, 0x0003000f, 0x00030010, 0x00030011, 0x00030012, 0x00030013, 0x00040000,
        0x00040001, 0x00040002, 0x00040003, 0x00040004, 0x00040005, 0x00040006, 0x00040007,
        0x00040008, 0x00040009, 0x0004000a, 0x0004000b, 0x0004000c, 0x0004000d, 0x0004000e,
        0x0004000f, 0x00040010, 0x00040011, 0x00040012, 0x00040013, 0x00040014, 0x00050001,
        0x00050002, 0x00050003, 0x00050004, 0x00060001, 0x00060002, 0x00060003, 0x00060004,
        0x00060005, 0x00060006, 0x00060007, 0x00060008, 0x00060009, 0x0006000a, 0x0006000b,
        0x0006000c, 0x0006000d, 0x0006000e, 0x0006000f, 0x00070001, 0x00070002, 0x00070003,
        0x00070004
    };
    uint32_t* end = errorCodes + (sizeof(errorCodes) / sizeof(errorCodes[0]));
    uint32_t* found = std::find(errorCodes, end, rawErrorCode);
    return (found == end) ? ErrorCode_Unknown : static_cast<ErrorCode>(*found);
}

const char* HostServiceException::getErrorCodeMessage(ErrorCode errorCode) const
{
    switch (errorCode)
    {
    case ErrorCode_None:
        return "No error";
    case ErrorCode_InvalidRandomSeedExchange:
        return "Invalid exchange attributes request or invalid exchange random seeds request";
    case ErrorCode_InvalidServerId:
        return "Invalid server ID";
    case ErrorCode_InvalidRequestId:
        return "Invalid request ID";
    case ErrorCode_InvalidRandomSeed:
        return "Random seeds must be in the range (0, 0xdfffffffffffffff]";
    case ErrorCode_RandomSeedMissing:
        return "Random seeds required when doing password substitution";
    case ErrorCode_InvalidPasswordEncryptIndicator:
        return "Invalid password encrypt indicator";
    case ErrorCode_InvalidUserIdLength:
        return "Invalid user ID length";
    case ErrorCode_InvalidPasswordLength:
        return "Invalid password length";
    case ErrorCode_InvalidClientVersionOrSendReplyIndicator:
        return "Invalid client version or invalid send reply indicator";
    case ErrorCode_InvalidRequestData:
        return "Invalid request data";
    case ErrorCode_InvalidSignonRequest:
        return "Invalid signon request";
    case ErrorCode_InvalidChangePasswordRequest:
        return "Invalid change password request";
    case ErrorCode_InvalidOldPassword:
        return "Invalid old password";
    case ErrorCode_InvalidNewPassword:
        return "Invalid new password";
    case ErrorCode_InvalidTokenType:
        return "Invalid token type on generate authentication token request";
    case ErrorCode_InvalidGenerateTokenRequest:
        return "Invalid generate authentication token request";
    case ErrorCode_InvalidTokenLength:
        return "Invalid authentication token length";
    case ErrorCode_InvalidGenerateTokenForOtherUserRequest:
        return "Invalid generate authentication token for another user";
    case ErrorCode_UnknownUserId:
        return "Unknown user ID";
    case ErrorCode_UserIdValidButRevoked:
        return "User ID valid, but revoked";
    case ErrorCode_UserIdMismatch:
        return "User profile mismatch";
    case ErrorCode_NewPasswordTooLong:
        return "New password too long";
    case ErrorCode_NewPasswordTooShort:
        return "New password too short";
    case ErrorCode_NewPasswordRepeatCharacter:
        return "New password contains character used more than once";
    case ErrorCode_NewPasswordAdjacentDigits:
        return "New password has adjacent digits";
    case ErrorCode_NewPasswordCharacterConsecutiveRepeat:
        return "New password contains a character repeated consecutively";
    case ErrorCode_NewPasswordPreviouslyUsed:
        return "New password has been used before";
    case ErrorCode_NewPasswordMissingNumeral:
        return "New password is missing a numeral";
    case ErrorCode_NewPasswordInvalidCharacter:
        return "New password contains an invalid character";
    case ErrorCode_NewPasswordDisallowed:
        return "New password appears on a list of disallowed passwords";
    case ErrorCode_NewPasswordContainsUserId:
        return "New password contains the user ID";
    case ErrorCode_PasswordIncorrect:
        return "Password or passphrase incorrect";
    case ErrorCode_UserIdDisabledOnNextFailedSignon:
        return "User profile will be disabled on next invalid password or passphrase";
    case ErrorCode_PasswordCorrectButExpired:
        return "Password correct, but expired";
    case ErrorCode_PasswordPreV2R2Encryption:
        return "Password uses pre-V2R2 encryption";
    case ErrorCode_NewPasswordCharacterInSamePositionAsOld:
        return "New password or passphrase contains a character in the same position as the previous password or passphrase";
    case ErrorCode_PasswordIsStarNone:
        return "Password is *NONE";
    case ErrorCode_PasswordValidationFailed:
        return "Password validation program failed the request";
    case ErrorCode_PasswordChangeNotAllowed:
        return "Password change not allowed at this time";
    case ErrorCode_InvalidPasswordValue:
        return "Password value is not valid";
    case ErrorCode_GeneralSecurity:
        return "General security failure";
    case ErrorCode_IncorrectProgramDataLength:
        return "QYSMPUT failed due to incorrect program data length";
    case ErrorCode_ServerJobTimeout:
        return "QYSMPUT failed because no response was received from the server job";
    case ErrorCode_ServerJobNotStarted:
        return "QYSMPUT failed because the server job could not be started";
    case ErrorCode_PrestartJobNotStarted:
        return "QYSMPUT failed because the prestart job could not be started";
    case ErrorCode_SubsystemFailure:
        return "QYSMPUT failed because of subsystem problems";
    case ErrorCode_ServerJobEnding:
        return "QYSMPUT failed because the server job is ending";
    case ErrorCode_ReceiverAreaTooSmall:
        return "QYSMPUT failed because the receiver area is too small";
    case ErrorCode_ConnectionNotPassed:
        return "QYSMPUT failed for unknown reasons";
    case ErrorCode_NonexistentServerJobUserId:
        return "QYSMPUT failed because the user profile for the server job does not exist";
    case ErrorCode_ServerJobAuthorityFailure:
        return "QYSMPUT failed because of authority problems with the user profile of the server job";
    case ErrorCode_ServerJobProgramNotFound:
        return "QYSMPUT failed because the program for the server job could not be found";
    case ErrorCode_DaemonJobServerJobLibraryAuthority:
        return "QYSMPUT failed because the daemon job is not authorized to use the library that contains the server job";
    case ErrorCode_DaemonJobServerJobProgramAuthority:
        return "QYSMPUT failed because the daemon job is not authorized to the server job program";
    case ErrorCode_UserNotAuthorizedToGenerateToken:
        return "User not authorized to generate a token for another user";
    case ErrorCode_NoMemoryToGenerateToken:
        return "No memory is available to allocate space needed for authorization";
    case ErrorCode_TextConversion:
        return "An error occurred converting data between code pages";
    case ErrorCode_EimInterface:
        return "An error occurred using EIM interfaces";
    case ErrorCode_CryptoInterface:
        return "An error occurred using cryptographic interfaces";
    case ErrorCode_UnsupportedTokenVersion:
        return "This version of code does not support this version of token";
    case ErrorCode_PublicKeyNotFound:
        return "Public key not found";
    case ErrorCode_ExitPointProcessingFailure:
        return "Could not process exit point";
    case ErrorCode_CouldNotResolveExitProgram:
        return "Could not resolve exit program";
    case ErrorCode_CouldNotCallExitProgram:
        return "Could not call user exit program";
    case ErrorCode_ExitProgramDeniedRequest:
        return "User exit program denied the request";
    case ErrorCode_InvalidToken:
        return "Profile token or identity token not valid";
    case ErrorCode_TooManyTokens:
        return "The maximum number of profile tokens for the system has already been generated";
    case ErrorCode_InvalidTimeoutInterval:
        return "Invalid timeout interval";
    case ErrorCode_InvalidTokenRequest:
        return "Invalid type of profile token request";
    case ErrorCode_TokenNotRegenerable:
        return "Exisint profile token is not regenerable, and can't be used to generate a new profile token";
    case ErrorCode_KerberosConsistencyCheckFailure:
        return "Kerberos ticket not valid due to a consistency check failure";
    case ErrorCode_AuthenticationMechanismsNotSupported:
        return "Requested authentication mechanisms not supported";
    case ErrorCode_InvalidCredentials:
        return "Credentials not available or not valid for this context";
    case ErrorCode_InvalidKerberosTicketSignature:
        return "Kerberos token or identity token contains incorrect signature";
    case ErrorCode_InvalidKerberosCredentials:
        return "Authentication token redentials no longer valid";
    case ErrorCode_InconsistentKerberosTicket:
        return "Consistency checks on the authentication token credential structure failed";
    case ErrorCode_VerificationFailure:
        return "Authentication token verification routine failed";
    case ErrorCode_EimConfiguration:
        return "EIM configuration errror";
    case ErrorCode_InvalidKerberosSystemProfile:
        return "Kerberos principal maps to a user profile that cannot be used to sign on";
    case ErrorCode_PrincipalMapsToMultipleUserIds:
        return "Kerberos princiapl maps to multiple user profile names";
    case ErrorCode_CannotConnectToEimDomain:
        return "Cannot connect to the system EIM domain";
    case ErrorCode_CannotChangeCCSIDForEimRequests:
        return "Can not change the CCSID to use for EIM requests to 13488";
    case ErrorCode_CannotObtainEimRegistryName:
        return "Cannot obtain the EIM registry domain";
    case ErrorCode_NoWebsphereToOs400ProfileMappingExists:
        return "No mapping exists between the WebSphere Portal user identity and an i5/OS user profile";
    case ErrorCode_Unknown:
        break;
    }
    return "Unknown error";
}

HostServiceException::HostServiceException(const HostServiceException& except)
    : Exception(except), m_rawErrorCode(except.m_rawErrorCode)
{
}

HostServiceException& HostServiceException::operator= (const HostServiceException& except)
{
    Exception::operator=(except);
    m_rawErrorCode = except.m_rawErrorCode;
    return *this;
}

Exception* HostServiceException::duplicate() const
{
    return new HostServiceException(*this);
}

}
