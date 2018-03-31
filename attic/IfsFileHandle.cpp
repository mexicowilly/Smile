#include "smile/IfsFileHandle.hpp"
#include "FileClient.hpp"

namespace
{

const uint32_t INVALID_HANDLE = UINT32_MAX;

}

namespace smile
{

IfsFileHandle::IfsFileHandle(AS400& as400, const IfsObjectName& fileName)
    : m_as400(&as400),
      m_fileClient(new FileClient(as400)),
      m_fileName(fileName),
      m_handle(INVALID_HANDLE),
      m_offset(0)

{
}

IfsFileHandle::IfsFileHandle(IfsFile& file)
    : m_as400(&file.getAS400()),
      m_fileClient(new FileClient(*m_as400)),
      m_fileName(file.getName()),
      m_handle(INVALID_HANDLE),
      m_offset(0)
{
}

IfsFileHandle::IfsFileHandle(const IfsFileHandle& handle)
    : m_as400(handle.m_as400),
      m_fileClient(new FileClient(*m_as400)),
      m_fileName(handle.m_fileName),
      m_handle(handle.m_handle),
      m_offset(handle.m_offset)
{
}

IfsFileHandle::~IfsFileHandle()
{
    delete m_fileClient;
}

IfsFileHandle& IfsFileHandle::operator= (const IfsFileHandle& handle)
{
    if (&handle != this)
    {
        m_as400 = handle.m_as400;
        delete m_fileClient;
        m_fileClient = new FileClient(*m_as400);
        m_fileName = handle.m_fileName;
        m_handle = handle.m_handle;
        m_offset = handle.m_offset;
    }
    return *this;
}

void IfsFileHandle::close()
{
    if (m_handle != INVALID_HANDLE)
    {
        m_fileClient->close(m_handle);
        m_handle = INVALID_HANDLE;
    }
}

bool IfsFileHandle::isOpen() const
{
    return m_handle != INVALID_HANDLE;
}

IfsLockHandle IfsFileHandle::lock(uint64_t offset, uint64_t length)
{
    if (m_handle == INVALID_HANDLE)
        throw InvalidStateException(__FILE__, __LINE__, "The file handle is not open");
    return m_fileClient->lock(m_handle, offset, length);
}

void IfsFileHandle::open(Access accessMode, OpenMode openMode)
{
    if (m_handle != INVALID_HANDLE)
        throw InvalidStateException(__FILE__, __LINE__, "The file handle is already open");
    std::pair<uint32_t, uint64_t> info =
        m_fileClient->open(m_fileName.getAbsolutePath(), accessMode, openMode);
    m_handle = info.first;
    m_offset = info.second;
}

bool IfsFileHandle::read(uint8_t* buffer, size_t& byteCount)
{
    if (m_handle == INVALID_HANDLE)
        throw InvalidStateException(__FILE__, __LINE__, "The file handle is not open");
    return m_fileClient->read(m_handle, m_offset, buffer, byteCount);
}

uint64_t IfsFileHandle::seek(SeekFrom seekFrom, int64_t offset)
{
    if (seekFrom == SeekFrom_Beginning)
        m_offset = offset;
    else if (seekFrom == SeekFrom_Current)
        m_offset += offset;
    else if (seekFrom == SeekFrom_End)
        m_offset = m_fileClient->getSize(m_fileName.getAbsolutePath()) - offset;
    return m_offset;
}

void IfsFileHandle::sync()
{
    if (m_handle == INVALID_HANDLE)
        throw InvalidStateException(__FILE__, __LINE__, "The file handle is not open");
    m_fileClient->sync(m_handle);
}

void IfsFileHandle::unlock(const IfsLockHandle& handle)
{
    m_fileClient->unlock(handle);
}

void IfsFileHandle::write(const uint8_t* bytes, size_t byteCount)
{
    if (m_handle == INVALID_HANDLE)
        throw InvalidStateException(__FILE__, __LINE__, "The file handle is not open");
    m_fileClient->write(m_handle, m_offset, bytes, byteCount);
}

}
