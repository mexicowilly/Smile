#if !defined(SMILE_IFSFILEHANDLE_HPP__)
#define SMILE_IFSFILEHANDLE_HPP__

#include <smile/AS400.hpp>
#include <smile/IfsLockHandle.hpp>
#include <smile/IfsFile.hpp>

namespace smile
{

class FileClient;

class IfsFileHandle
{
public:
    enum Access
    {
        Access_Read         = 1,
        Access_Write        = 2,
        Access_ReadWrite    = 3
    };

    enum OpenMode
    {
        OpenMode_CreateAppend       = 0x01,
        OpenMode_CreateTruncate     = 0x02,
        OpenMode_MustCreate         = 0x04,
        OpenMode_Append             = 0x08,
        OpenMode_Truncate           = 0x10,
        OpenMode_Beginning          = OpenMode_Truncate
    };

    enum SeekFrom
    {
        SeekFrom_Beginning,
        SeekFrom_Current,
        SeekFrom_End
    };

    IfsFileHandle(AS400& as400, const IfsObjectName& fileName);
    IfsFileHandle(IfsFile& file);
    IfsFileHandle(const IfsFileHandle& handle);
    ~IfsFileHandle();

    IfsFileHandle& operator= (const IfsFileHandle& handle);

    void close();
    const IfsObjectName& getName() const;
    bool isOpen() const;
    IfsLockHandle lock(uint64_t offset, uint64_t length);
    IfsLockHandle lock(uint64_t length);
    void open(Access accessMode, OpenMode openMode);
    bool read(uint8_t* buffer, size_t& byteCount);
    uint64_t seek(SeekFrom seekFrom, int64_t offset);
    uint64_t tell() const;
    void sync();
    void unlock(const IfsLockHandle& handle);
    void write(const uint8_t* bytes, size_t byteCount);

private:
    AS400* m_as400;
    FileClient* m_fileClient;
    IfsObjectName m_fileName;
    uint32_t m_handle;
    uint64_t m_offset;
};

inline const IfsObjectName& IfsFileHandle::getName() const
{
    return m_fileName;
}

inline IfsLockHandle IfsFileHandle::lock(uint64_t length)
{
    return lock(m_offset, length);
}

inline uint64_t IfsFileHandle::tell() const
{
    return m_offset;
}

}

#endif
