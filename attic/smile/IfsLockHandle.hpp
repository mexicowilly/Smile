#if !defined(SMILE_IFSLOCKHANDLE_HPP__)
#define SMILE_IFSLOCKHANDLE_HPP__

#include <inttypes.h>

namespace smile
{

class IfsLockHandle
{
public:
    IfsLockHandle(const IfsLockHandle& handle);

    IfsLockHandle& operator= (const IfsLockHandle& handle);

protected:
    friend class FileClient;

    IfsLockHandle(uint32_t fileHandle, uint64_t offset, uint64_t length);

private:
    uint32_t m_fileHandle;
    uint64_t m_offset;
    uint64_t m_length;
};

}

#endif
