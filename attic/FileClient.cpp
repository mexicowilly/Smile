#include "FileClient.hpp"
#include "FileExchangeAttributesRequest.hpp"
#include "FileExchangeAttributesReply.hpp"
#include "ListAttributesRequest.hpp"
#include "TypeInfo.hpp"
#include "OpenFileRequest.hpp"
#include "OpenFileReply.hpp"
#include "CloseFileRequest.hpp"
#include "CloseFileReply.hpp"
#include "CommitFileRequest.hpp"
#include "ReadFileRequest.hpp"
#include "ReadFileReply.hpp"
#include "WriteFileRequest.hpp"
#include "WriteFileReply.hpp"
#include "LockBytesRequest.hpp"
#include "LockBytesReply.hpp"
#include "UnlockBytesRequest.hpp"
#include "CreateDirectoryRequest.hpp"
#include "ServicePool.hpp"
#include "DeletePointer.hpp"

namespace
{

const size_t SMILE_READ_MAX = 16000000;

}

namespace smile
{

FileClient::FileClient(AS400& as400)
    : ServiceFactory(as400),
      m_fileService(0)
{
}

FileClient::~FileClient()
{
    ServicePool::getInstance().checkIn(m_fileService);
}

void FileClient::close(uint32_t fileHandle)
{
    connect();
    CloseFileRequest request(fileHandle);
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    CloseFileReply* closeFile = dynamic_cast<CloseFileReply*>(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (closeFile != 0)
    {
        if (closeFile->getReturnCode() != 0)
        {
            throw FileServiceException(__FILE__, __LINE__,
                                       "Error closing file", closeFile->getReturnCode());
        }
    }
    else if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    else
    {
        std::ostringstream stream;
        stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
}

void FileClient::connect()
{
    if (m_fileService == 0)
        m_fileService = dynamic_cast<FileService*>(ServicePool::getInstance().checkOut(*this));
}

void FileClient::createDirectory(const Text& directory)
{
    connect();
    CreateDirectoryRequest request(directory.convert(m_fileService->getCCSID()), m_fileService->getCCSID());
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    else
    {
        std::ostringstream stream;
        stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
}

HostService* FileClient::createHostService()
{
    FileService* service = new FileService(m_as400);
    try
    {
        service->connect();
        uint32_t version = m_as400.getVersion().getPacked();
        uint16_t dataStreamLevel = 0;
        if (version >= 0x00060100)
            dataStreamLevel = 16;
        else if (version >= 0x00050300)
            dataStreamLevel = 8;
        else if (version >= 0x00040500)
            dataStreamLevel = 2;
        std::vector<int> preferredCCSIDs;
        if (version >= 0x00050300)
        {
            preferredCCSIDs.push_back(1200);
            preferredCCSIDs.push_back(13488);
            preferredCCSIDs.push_back(61952);
        }
        else if (version >= 0x00040500)
        {
            preferredCCSIDs.push_back(13488);
            preferredCCSIDs.push_back(61952);
        }
        else
        {
            preferredCCSIDs.push_back(61952);
        }
        FileExchangeAttributesRequest request(preferredCCSIDs, dataStreamLevel);
        FileExchangeAttributesReply* reply;
        reply = service->sendAndReceive<FileExchangeAttributesReply>(request);
        service->imbueAttributes(*reply);
        delete reply;
    }
    catch (...)
    {
        delete service;
        throw;
    }
    return service;
}

FileClient::Existence FileClient::exists(const Text& object)
{
    std::vector<ListAttributesReply*> replies;
    try
    {
        replies = listAttributes(object);
    }
    catch (OS400ErrnoException& o4e)
    {
        OS400ErrnoException::Errno code = o4e.getErrnoCode();
        if (code != OS400ErrnoException::Errno_ENOENT ||
            code != OS400ErrnoException::Errno_ENOENT1)
        {
            return Existence_None;
        }
        throw;
    }
    catch (FileServiceException& fse)
    {
        FileServiceException::ErrorCode code = fse.getErrorCode();
        if (code == FileServiceException::ErrorCode_FileNotFound ||
            code == FileServiceException::ErrorCode_PathNotFound)
        {
            return Existence_None;
        }
        throw;
    }
    Existence existence = Existence_None;
    if (!replies.empty())
    {
        existence = replies[0]->isDirectory() ? Existence_Directory : Existence_File;
        std::for_each(replies.begin(), replies.end(), DeletePointer<ListAttributesReply>());
    }
    return existence;
}

uint16_t FileClient::getServiceIdentifier()
{
    return FileService::IDENTIFIER;
}

uint64_t FileClient::getSize(const Text& fileName)
{
    std::vector<ListAttributesReply*> replies = listAttributes(fileName);
    uint64_t result = 0;
    if (!replies.empty())
    {
        result = replies[0]->getSize(m_fileService->getDataStreamLevel());
        std::for_each(replies.begin(), replies.end(), DeletePointer<ListAttributesReply>());
    }
    return result;
}

void FileClient::handleReturnCodeReply(const ReturnCodeReply& reply)
{
    uint32_t rc = reply.getReturnCode();
    if (rc != 0)
    {
        OS400ErrnoException::Errno errnoCode = OS400ErrnoException::convertErrno(rc);
        if (errnoCode != OS400ErrnoException::Errno_ESMILEUNKNOWN)
        {
            throw OS400ErrnoException(__FILE__, __LINE__,
                                      "IFS file error", rc);
        }
        else
        {
            if (FileServiceException::convertErrorCode(rc) !=
                FileServiceException::ErrorCode_NoMoreFiles)
            {
                throw FileServiceException(__FILE__, __LINE__,
                                           "IFS file error", rc);
            }
        }
    }
}

std::vector<ListAttributesReply*> FileClient::listAttributes(const Text& pattern)
{
    connect();
    ListAttributesRequest request(pattern.convert(m_fileService->getCCSID()), m_fileService->getCCSID());
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    std::vector<ListAttributesReply*> output;
    try
    {
        while (true)
        {
            ListAttributesReply* listAttr = dynamic_cast<ListAttributesReply*>(reply);
            ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
            if (listAttr != 0)
            {
                autoReply.release();
                output.push_back(listAttr);
            }
            else if (returnCode != 0)
            {
                handleReturnCodeReply(*returnCode);
            }
            else
            {
                std::ostringstream stream;
                stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
                throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
            }
            if (reply->isEndOfChain())
                break;
            PacketReply* rawReply;
            try
            {
                rawReply = m_fileService->receive(request.getCorrelation());
            }
            catch (...)
            {
                delete m_fileService;
                m_fileService = 0;
                throw;
            }
            reply = dynamic_cast<FileServiceReply*>(rawReply);
            if (reply == 0)
            {
                std::ostringstream stream;
                stream << "Unexpected reply type: " << TypeInfo::getName(typeid(*rawReply));
                delete rawReply;
                throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
            }
            autoReply.reset(reply);
        }
    }
    catch (...)
    {
        std::for_each(output.begin(), output.end(), DeletePointer<ListAttributesReply>());
        throw;
    }
    return output;
}

std::vector<std::pair<Text, bool> > FileClient::listDirectory(const Text& directory)
{
    static UChar slashStarChars[] = { 0x002f, 0x002a };

    Text pattern(directory);
    if (pattern.getAt(pattern.getLength() - 1) != 0x002f)
        pattern += Text(slashStarChars, 2);
    else
        pattern += Text(slashStarChars + 1, 1);
    std::vector<ListAttributesReply*> replies = listAttributes(pattern);
    std::vector<std::pair<Text, bool> > output;
    Text oneDot(SMILE_WSTR("."));
    Text twoDots(SMILE_WSTR(".."));
    for (std::vector<ListAttributesReply*>::iterator itor = replies.begin();
         itor != replies.end();
         itor++)
    {
        Text cur((*itor)->getObjectName(), m_fileService->getCCSID());
        if (cur != oneDot && cur != twoDots)
            output.push_back(std::make_pair(cur, (*itor)->isDirectory()));
        delete *itor;
    }
    return output;
}

IfsLockHandle FileClient::lock(uint32_t fileHandle, uint64_t offset, uint64_t length)
{
    connect();
    LockBytesRequest request(fileHandle, offset, length, m_fileService->getDataStreamLevel());
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    LockBytesReply* lockBytes = dynamic_cast<LockBytesReply*>(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (lockBytes != 0)
    {
        FileServiceException::ErrorCode code =
            FileServiceException::convertErrorCode(lockBytes->getReturnCode());
        if (code != FileServiceException::ErrorCode_NoError)
        {
            throw FileServiceException(__FILE__, __LINE__,
                                       "Could not lock file bytes", lockBytes->getReturnCode());
        }
        return IfsLockHandle(fileHandle, offset, length);
    }
    else if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    std::ostringstream stream;
    stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
    throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
}

std::pair<uint32_t, uint64_t> FileClient::open(const Text& fileName,
                                               IfsFileHandle::Access accessMode,
                                               IfsFileHandle::OpenMode openMode)
{
    connect();
    OpenFileRequest request(fileName.convert(m_fileService->getCCSID()),
                            m_fileService->getCCSID(),
                            accessMode,
                            openMode,
                            m_fileService->getDataStreamLevel());
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    OpenFileReply* openFile = dynamic_cast<OpenFileReply*>(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (openFile != 0)
    {
        uint64_t currentOffset = 0;
        if (openMode == IfsFileHandle::OpenMode_Append ||
            openMode == IfsFileHandle::OpenMode_CreateAppend)
        {
            currentOffset = openFile->getFileSize(m_fileService->getDataStreamLevel());
        }
        return std::make_pair(openFile->getFileHandle(), currentOffset);
    }
    else if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    std::ostringstream stream;
    stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
    throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
}

bool FileClient::read(uint32_t fileHandle,
                      uint64_t& offset,
                      uint8_t* buf,
                      size_t& byteCount)
{
    size_t byteCountToRead = byteCount;
    if (byteCountToRead == 0)
        return true;
    connect();
    byteCount = 0;
    std::auto_ptr<FileServiceReply> autoReply;
    bool endOfFile = false;
    while (byteCountToRead > 0 && !endOfFile)
    {
        ReadFileRequest request(fileHandle,
                                offset,
                                std::min(byteCountToRead, SMILE_READ_MAX),
                                m_fileService->getDataStreamLevel());
        FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
        autoReply.reset(reply);
        while (true)
        {
            ReadFileReply* readFile = dynamic_cast<ReadFileReply*>(reply);
            ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
            if (readFile != 0)
            {
                std::vector<uint8_t> data = readFile->getData();
                if (data.empty())
                {
                    endOfFile = true;
                }
                else
                {
                    memcpy(buf + byteCount, &data[0], data.size());
                    byteCount += data.size();
                    byteCountToRead -= data.size();
                    offset += data.size();
                }
            }
            else if (returnCode != 0)
            {
                FileServiceException::ErrorCode err =
                    FileServiceException::convertErrorCode(returnCode->getReturnCode());
                if (err == FileServiceException::ErrorCode_NoMoreData)
                    endOfFile = true;
                else
                    handleReturnCodeReply(*returnCode);
            }
            else
            {
                std::ostringstream stream;
                stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
                throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
            }
            if (reply->isEndOfChain())
                break;
            PacketReply* rawReply;
            try
            {
                rawReply = m_fileService->receive(request.getCorrelation());
            }
            catch (...)
            {
                delete m_fileService;
                m_fileService = 0;
                throw;
            }
            reply = dynamic_cast<FileServiceReply*>(rawReply);
            if (reply == 0)
            {
                std::ostringstream stream;
                stream << "Unexpected reply type: " << TypeInfo::getName(typeid(*rawReply));
                delete rawReply;
                throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
            }
            autoReply.reset(reply);
        }
    }
    return !endOfFile;
}

void FileClient::sync(uint32_t fileHandle)
{
    connect();
    CommitFileRequest request(fileHandle);
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    else
    {
        std::ostringstream stream;
        stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
}

void FileClient::unlock(const IfsLockHandle& handle)
{
    connect();
    UnlockBytesRequest request(handle.m_fileHandle,
                               handle.m_offset,
                               handle.m_length,
                               m_fileService->getDataStreamLevel());
    FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
    std::auto_ptr<FileServiceReply> autoReply(reply);
    ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
    if (returnCode != 0)
    {
        handleReturnCodeReply(*returnCode);
    }
    else
    {
        std::ostringstream stream;
        stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
}

void FileClient::write(uint32_t fileHandle,
                       uint64_t& offset,
                       const uint8_t* bytes,
                       size_t byteCount)
{
    if (byteCount == 0)
        return;
    connect();
    size_t totalWritten = 0;
    while (byteCount > 0)
    {
        size_t thisWriteCount = std::min(m_fileService->getMaxDataBlockSize(), byteCount);
        WriteFileRequest request(fileHandle,
                                 offset,
                                 bytes + totalWritten,
                                 thisWriteCount,
                                 m_fileService->getCCSID(),
                                 m_fileService->getDataStreamLevel(),
                                 byteCount > thisWriteCount);
        FileServiceReply* reply = m_fileService->sendAndReceive<FileServiceReply>(request);
        std::auto_ptr<FileServiceReply> autoReply(reply);
        WriteFileReply* writeFile = dynamic_cast<WriteFileReply*>(reply);
        ReturnCodeReply* returnCode = dynamic_cast<ReturnCodeReply*>(reply);
        if (writeFile != 0)
        {
            if (writeFile->getReturnCode() != 0)
            {
                throw FileServiceException(__FILE__, __LINE__,
                                           "Error reading file", writeFile->getReturnCode());
            }
            size_t thisWritten = byteCount - writeFile->getLengthNotWritten();
            offset += thisWritten;
            totalWritten += thisWritten;
            byteCount -= thisWritten;
        }
        else if (returnCode != 0)
        {
            handleReturnCodeReply(*returnCode);
        }
        else
        {
            std::ostringstream stream;
            stream << "Received unexpected reply of type " << TypeInfo::getName(typeid(*reply));
            throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
        }
    }
}

}
