#include "FileService.hpp"
#include "FileExchangeAttributesReply.hpp"
#include "OpenFileReply.hpp"
#include "ReturnCodeReply.hpp"
#include "ListAttributesReply.hpp"
#include "CloseFileReply.hpp"
#include "ReadFileReply.hpp"
#include "WriteFileReply.hpp"
#include "LockBytesReply.hpp"

namespace smile
{

FileService::FileService(AS400Impl& as400)
    : HandshakeService(as400), m_maxDataBlockSize(0), m_ccsid(0), m_dataStreamLevel(0)
{
}

PacketReply* FileService::convertPacketToReply(const AccessPacket& packet)
{
    PacketReply* result;
    switch (packet.getRequestReplyId())
    {
    case 0x8001:
        result = new ReturnCodeReply(packet);
        break;
    case 0x8002:
        result = new OpenFileReply(packet);
        break;
    case 0x8005:
        result = new ListAttributesReply(packet);
        break;
    case 0x8003:
        result = new ReadFileReply(packet);
        break;
    case 0x8004:
        result = new CloseFileReply(packet);
        break;
    case 0x8006:
        // Create dir handle
    case 0x8007:
        // Query space
        assert(false);
        break;
    case 0x800a:
        result = new LockBytesReply(packet);
        break;
    case 0x800b:
        result = new WriteFileReply(packet);
        break;
    case 0x8009:
        result = new FileExchangeAttributesReply(packet);
        break;
    default:
        result = HandshakeService::convertPacketToReply(packet);
        break;
    }
    return result;
}

uint16_t FileService::getIdentifier() const
{
    return IDENTIFIER;
}

const char* FileService::getName() const
{
    return "as-file";
}

void FileService::imbueAttributes(FileExchangeAttributesReply& reply)
{
    m_ccsid = reply.getPreferredCCSID();
    m_dataStreamLevel = reply.getDataStreamLevel();
    m_maxDataBlockSize = reply.getMaxDataBlockSize();
}

}
