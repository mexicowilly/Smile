#if !defined(SMILE_FILESERVICE_HPP__)
#define SMILE_FILESERVICE_HPP__

#include "HandshakeService.hpp"
#include "FileExchangeAttributesReply.hpp"

namespace smile
{

class FileService : public HandshakeService
{
public:
    static const uint16_t IDENTIFIER = 0xe002;

    enum PatternMatching
    {
        PatternMatching_Posix           = 0,
        PatternMatching_PosixAll        = 1,
        PatternMatching_PC              = 2
    };

    FileService(AS400Impl& as400);

    int getCCSID() const;
    uint16_t getDataStreamLevel() const;
    virtual uint16_t getIdentifier() const;
    size_t getMaxDataBlockSize() const;
    virtual const char* getName() const;
    void imbueAttributes(FileExchangeAttributesReply& reply);

protected:
    virtual PacketReply* convertPacketToReply(const AccessPacket& packet);

private:
    size_t m_maxDataBlockSize;
    int m_ccsid;
    uint16_t m_dataStreamLevel;
};

inline int FileService::getCCSID() const
{
    return m_ccsid;
}

inline uint16_t FileService::getDataStreamLevel() const
{
    return m_dataStreamLevel;
}

inline size_t FileService::getMaxDataBlockSize() const
{
    return m_maxDataBlockSize;
}

}

#endif
