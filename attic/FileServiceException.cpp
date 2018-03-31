#include <smile/FileServiceException.hpp>
#include <sstream>
#include <algorithm>

namespace smile
{

FileServiceException::FileServiceException(const char* const fileName,
                                           int lineNumber,
                                           const char* const msg,
                                           uint32_t rawErrorCode)
    : Exception(fileName, lineNumber), m_rawErrorCode(rawErrorCode)
{
    ErrorCode code = convertErrorCode(rawErrorCode);
    std::ostringstream stream;
    stream << msg << ": " << getErrorCodeMessage(code);
    if (code == ErrorCode_UnknownToSmile)
        stream << " (" << std::hex << std::showbase << rawErrorCode << ")";
    m_what = new char[stream.str().length() + 1];
    strcpy(m_what, stream.str().c_str());
}

FileServiceException::FileServiceException(const FileServiceException& except)
    : Exception(except), m_rawErrorCode(except.m_rawErrorCode)
{
}

FileServiceException& FileServiceException::operator= (const FileServiceException& except)
{
    Exception::operator=(except);
    m_rawErrorCode = except.m_rawErrorCode;
    return *this;
}

Exception* FileServiceException::duplicate() const
{
    return new FileServiceException(*this);
}

FileServiceException::ErrorCode FileServiceException::convertErrorCode(uint32_t rawErrorCode)
{
    static uint32_t errorCodes[] =
    {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        20, 21, 22, 23, 24, 25, 32, 33
    };
    static uint32_t* end = errorCodes + (sizeof(errorCodes) / sizeof(errorCodes[0]));

    uint32_t* found = std::find(errorCodes, end, rawErrorCode);
    return (found == end) ? ErrorCode_UnknownToSmile : static_cast<ErrorCode>(*found);
}

const char* FileServiceException::getErrorCodeMessage(ErrorCode errorCode) const
{
    switch (errorCode)
    {
    case ErrorCode_NoError:
        return "No error";
    case ErrorCode_FileInUse:
        return "File in use";
    case ErrorCode_FileNotFound:
        return "File not found";
    case ErrorCode_PathNotFound:
        return "Path not found";
    case ErrorCode_DuplicateDirEntryName:
        return "Duplicate directory entry name";
    case ErrorCode_AccessDeniedToDirEntry:
        return "Access denied to directory entry";
    case ErrorCode_InvalidHandle:
        return "Invalid handle";
    case ErrorCode_InvalidDirEntryName:
        return "Invalid directory entry name";
    case ErrorCode_InvalidAttributeName:
        return "Invalid attribute name";
    case ErrorCode_DirIsNotEmpty:
        return "Directory is not empty";
    case ErrorCode_FileSubstreamInUse:
        return "File substream in use";
    case ErrorCode_ResourceLimitExceeded:
        return "Resource limit exceeded";
    case ErrorCode_ResourceNotAvailable:
        return "Resource not available";
    case ErrorCode_AccessDeniedToRequest:
        return "Access denied to request";
    case ErrorCode_DirEntryDamaged:
        return "Directory entry damaged";
    case ErrorCode_InvalidConnection:
        return "Invalid connection";
    case ErrorCode_InvalidRequest:
        return "Invalid request";
    case ErrorCode_DataStreamSyntaxError:
        return "Data stream syntax error";
    case ErrorCode_NoMoreFiles:
        return "No more files";
    case ErrorCode_ParameterNotSupported:
        return "Parameter not supported";
    case ErrorCode_ParameterValueNotSupported:
        return "Parameter value not supported";
    case ErrorCode_CannotConvertValue:
        return "Cannot convert value";
    case ErrorCode_NoMoreData:
        return "No more data";
    case ErrorCode_RequestNotSupported:
        return "Request not supported";
    case ErrorCode_InvalidUser:
        return "Invalid user";
    case ErrorCode_UnknownError:
        return "Unknown error";
    case ErrorCode_SharingViolation:
        return "Sharing violation";
    case ErrorCode_LockViolation:
        return "Lock violation";
    default:
        break;
    }
    return "Unexpected error code";
}

}
