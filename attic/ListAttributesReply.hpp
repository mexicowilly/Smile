#if !defined(SMILE_LISTATTRIBUTESREPLY_HPP__)
#define SMILE_LISTATTRIBUTESREPLY_HPP__

#include "FileServiceReply.hpp"

namespace smile
{

class ListAttributesReply : public FileServiceReply
{
public:
    ListAttributesReply(const AccessPacket& packet);

    virtual const char* getName() const;
    std::vector<uint8_t> getObjectName() const;
    uint64_t getSize(uint16_t dataStreamLevel) const;
    bool isDirectory() const;
};

inline ListAttributesReply::ListAttributesReply(const AccessPacket& packet)
    : FileServiceReply(packet)
{
}

}

#endif
