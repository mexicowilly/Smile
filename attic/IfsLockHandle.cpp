#include "smile/IfsLockHandle.hpp"

namespace smile
{

IfsLockHandle::IfsLockHandle(uint32_t fileHandle, uint64_t offset, uint64_t length)
    : m_fileHandle(fileHandle), m_offset(offset), m_length(length)
{
}

IfsLockHandle::IfsLockHandle(const IfsLockHandle& handle)
    : m_fileHandle(handle.m_fileHandle),
      m_offset(handle.m_offset),
      m_length(handle.m_length)
{
}

IfsLockHandle& IfsLockHandle::operator= (const IfsLockHandle& handle)
{
    m_fileHandle = handle.m_fileHandle;
    m_offset = handle.m_offset;
    m_length = handle.m_length;
    return *this;
}

}
