#if !defined(SMILE_OPENFILEREQUEST_HPP__)
#define SMILE_OPENFILEREQUEST_HPP__

#include "PacketRequest.hpp"
#include "smile/IfsFileHandle.hpp"

namespace smile
{

class OpenFileRequest : public PacketRequest
{
public:
    OpenFileRequest(const std::vector<uint8_t>& fileName,
                    int ccsid,
                    IfsFileHandle::Access accessMode,
                    IfsFileHandle::OpenMode openMode,
                    uint16_t dataStreamLevel);

    virtual const char* getName() const;
};

}

#endif
