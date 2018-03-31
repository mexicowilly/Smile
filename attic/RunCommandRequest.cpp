#include "RunCommandRequest.hpp"
#include "RemoteCommandService.hpp"

namespace smile
{

RunCommandRequest::RunCommandRequest(const std::vector<uint8_t>& command,
                                     uint16_t dataStreamLevel,
                                     OS400Message::MessageCount messageCount,
                                     int ccsid)
    : PacketRequest(((dataStreamLevel >= 10) ? 31 : 27) + command.size())
{
    m_packet.setServerId(RemoteCommandService::IDENTIFIER);
    m_packet.setRequestReplyId(0x1002);
    m_packet.setTemplateLength(1);
    uint8_t messageCountCode;
    if (dataStreamLevel < 7 && messageCount == OS400Message::MessageCount_All)
    {
        messageCountCode = 0;
    }
    else if (dataStreamLevel >= 10)
    {
        if (messageCount == OS400Message::MessageCount_UpTo10)
            messageCountCode = 3;
        else if (messageCount == OS400Message::MessageCount_All)
            messageCountCode = 4;
        else
            messageCountCode = 1;
    }
    else
    {
        if (messageCount == OS400Message::MessageCount_None)
            messageCountCode = 1;
        else if (messageCount == OS400Message::MessageCount_UpTo10)
            messageCountCode = 0;
        else
            messageCountCode = 2;
    }
    m_packet.setInt8(20, messageCountCode);
    if (dataStreamLevel >= 10)
    {
        m_packet.setInt32(21, 10 + command.size());
        m_packet.setInt16(25, 0x1104);
        m_packet.setInt32(27, ccsid);
        m_packet.setBytes(31, command);
    }
    else
    {
        m_packet.setInt32(21, 6 + command.size());
        m_packet.setInt16(25, 0x1101);
        m_packet.setBytes(27, command);
    }
}

const char* RunCommandRequest::getName() const
{
    return "run command";
}

}
