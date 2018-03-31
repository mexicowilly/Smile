#if !defined(SMILE_FILECLIENT_HPP__)
#define SMILE_FILECLIENT_HPP__

#include "FileService.hpp"
#include "ListAttributesReply.hpp"
#include "ReturnCodeReply.hpp"
#include "smile/IfsFileHandle.hpp"
#include "smile/IfsLockHandle.hpp"
#include "ServiceFactory.hpp"
#include "DeleteFileRequest.hpp"
#include "DeleteDirectoryRequest.hpp"

namespace smile
{

class FileClient : public ServiceFactory
{
public:
    enum Existence
    {
        Existence_None,
        Existence_File,
        Existence_Directory
    };

    FileClient(AS400& as400);
    ~FileClient();

    void close(uint32_t fileHandle);
    void createDirectory(const Text& directory);
    virtual HostService* createHostService();
    void deleteDirectory(const Text& dirName);
    void deleteFile(const Text& fileName);
    Existence exists(const Text& object);
    virtual uint16_t getServiceIdentifier();
    uint64_t getSize(const Text& fileName);
    std::vector<std::pair<Text, bool> > listDirectory(const Text& directory);
    IfsLockHandle lock(uint32_t fileHandle, uint64_t offset, uint64_t length);
    std::pair<uint32_t, uint64_t> open(const Text& fileName,
                                       IfsFileHandle::Access accessMode,
                                       IfsFileHandle::OpenMode openMode);
    bool read(uint32_t fileHandle,
              uint64_t& offset,
              uint8_t* buf,
              size_t& byteCount);
    void sync(uint32_t fileHandle);
    void unlock(const IfsLockHandle& handle);
    void write(uint32_t fileHandle,
               uint64_t& offset,
               const uint8_t* bytes,
               size_t byteCount);

private:
    void connect();
    template <class T> void deleteObject(const Text& fileName);
    void handleReturnCodeReply(const ReturnCodeReply& reply);
    std::vector<ListAttributesReply*> listAttributes(const Text& pattern);

    FileService* m_fileService;
};

inline void FileClient::deleteDirectory(const Text& dirName)
{
    deleteObject<DeleteDirectoryRequest>(dirName);
}

inline void FileClient::deleteFile(const Text& fileName)
{
    deleteObject<DeleteFileRequest>(fileName);
}

template <class T>
void FileClient::deleteObject(const Text& fileName)
{
    connect();
    T request(fileName.convert(m_fileService->getCCSID()), m_fileService->getCCSID());
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

}

#endif
