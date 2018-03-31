#include <smile/RemoteCommandServiceException.hpp>
#include <sstream>
#include <algorithm>

namespace smile
{

RemoteCommandServiceException::RemoteCommandServiceException(const char* const fileName,
                                                             int lineNumber,
                                                             const char* const msg,
                                                             uint16_t rawErrorCode)
    : Exception(fileName, lineNumber), m_rawErrorCode(rawErrorCode)
{
    ErrorCode code = convertErrorCode(rawErrorCode);
    std::ostringstream stream;
    stream << msg << ": " << getErrorCodeMessage(code);
    if (code == ErrorCode_Unknown)
        stream << " (" << std::hex << std::showbase << rawErrorCode << ")";
    m_what = new char[stream.str().length() + 1];
    strcpy(m_what, stream.str().c_str());
}

RemoteCommandServiceException::RemoteCommandServiceException(const RemoteCommandServiceException& except)
    : Exception(except), m_rawErrorCode(except.m_rawErrorCode)
{
}

RemoteCommandServiceException& RemoteCommandServiceException::operator= (const RemoteCommandServiceException& except)
{
    if (&except != this)
    {
        Exception::operator=(except);
        m_rawErrorCode = except.m_rawErrorCode;
    }
    return *this;
}

Exception* RemoteCommandServiceException::duplicate() const
{
    return new RemoteCommandServiceException(*this);
}

RemoteCommandServiceException::ErrorCode RemoteCommandServiceException::convertErrorCode(uint32_t rawErrorCode) const
{
    static uint16_t errorCodes[] =
    {
        0x0000, 0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0105, 0x0106, 0x0107, 0x0108,
        0x0200, 0x0201, 0x0202, 0x0203, 0x0204, 0x0205, 0x0300, 0x0301, 0x0302, 0x0303,
        0x0304, 0x0305, 0x0400, 0x0401, 0x0500, 0x0501
    };
    uint16_t* end = errorCodes + (sizeof(errorCodes) / sizeof(errorCodes[0]));
    uint16_t* found = std::find(errorCodes, end, rawErrorCode);
    return (found == end) ? ErrorCode_Unknown : static_cast<ErrorCode>(*found);
}

const char* RemoteCommandServiceException::getErrorCodeMessage(ErrorCode errorCode) const
{
    switch (errorCode)
    {
    case ErrorCode_None:
        return "No error";
    case ErrorCode_LimitedUser:
        return "Limited user";
    case ErrorCode_InvalidExchangeAttributesRequest:
        return "Invalid exchange attributes request";
    case ErrorCode_InvalidDataStreamLevel:
        return "Invalid data stream level";
    case ErrorCode_InvalidVersion:
        return "Invalid version";
    case ErrorCode_InvalidCCSID:
        return "Invalid coded character set identifier";
    case ErrorCode_InvalidNationalLanguageVersion:
        return "Invalid national language version, defaulting to primary";
    case ErrorCode_NationalLanguageVersionNotInstalled:
        return "National language version not installed, defaulting to primary";
    case ErrorCode_CantValidateNationalLanguageVersion:
        return "Cannot validate national language version";
    case ErrorCode_CantAddNLVLibraryToSystemLibraryList:
        return "Cannot add the national language version library to the system library list, possibly due to authority failure on CHGSYSLIBL command";
    case ErrorCode_UnableToProcessRequest:
        return "Unable to process request";
    case ErrorCode_InvalidLL:
        return "Invalid LL";
    case ErrorCode_InvalidServerId:
        return "Invalid server identifier";
    case ErrorCode_IncompleteData:
        return "Incomplete data";
    case ErrorCode_HostResourceError:
        return "Host resource error";
    case ErrorCode_InvalidRequestId:
        return "Invalid request identifier";
    case ErrorCode_ProcessExitPoint:
        return "Error retrieving the user exit point for exit point program processing, possibly because the exit program could not be resolved";
    case ErrorCode_InvalidRequest:
        return "Invalid request";
    case ErrorCode_InvalidParameter:
        return "Invalid parameter";
    case ErrorCode_MaximumExceeded:
        return "For command maximum length was exceeded, or for program call maximum number of parameters was exceeded";
    case ErrorCode_CallingUserExitProgram:
        return "Error calling user exit program";
    case ErrorCode_UserExitProgramDeniedRequest:
        return "User exit program denied the request";
    case ErrorCode_CommandFailed:
        return "Command failed, messages returned";
    case ErrorCode_InvalidCCSID_2:
        return "Invalid coded character set identifier";
    case ErrorCode_ResolvingProgram:
        return "Could not resolve the program to call";
    case ErrorCode_CallingProgram:
        return "Error occurred when calling the program";
    case ErrorCode_Unknown:
        break;
    }
    return "Unknown error";
}

}
