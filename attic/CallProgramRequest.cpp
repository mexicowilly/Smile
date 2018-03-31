#include "CallProgramRequest.hpp"
#include "RemoteCommandService.hpp"

namespace
{

size_t computeRequiredSize(smile::ProgramParameterList& parameters)
{
    size_t size = 0;
    for (unsigned i = 0; i < parameters.getSize(); i++)
    {
        smile::InParameter* inParam = dynamic_cast<smile::InParameter*>(&parameters[i]);
        size += ((inParam != 0) ? inParam->getInData().size() : 0) + 12;
    }
    return size;
}

}

namespace smile
{
CallProgramRequest::CallProgramRequest(const std::vector<uint8_t>& program,
                                       ProgramParameterList& parameters,
                                       uint16_t dataStreamLevel,
                                       OS400Message::MessageCount messageCount)
    : PacketRequest(43 + computeRequiredSize(parameters))
{
    m_packet.setServerId(RemoteCommandService::IDENTIFIER);
    m_packet.setTemplateLength(23);
    m_packet.setRequestReplyId(0x1003);
    m_packet.setBytes(20, program);
    uint8_t mcount = messageCount;
    if (dataStreamLevel < 7 && messageCount == OS400Message::MessageCount_All)
    {
        mcount = OS400Message::MessageCount_UpTo10;
    }
    else if (dataStreamLevel >= 10)
    {
        if (messageCount == OS400Message::MessageCount_UpTo10)
            mcount = 3;
        else if (messageCount == OS400Message::MessageCount_All)
            mcount = 4;
    }
    m_packet.setInt8(40, mcount);
    m_packet.setInt16(41, parameters.getSize());
    unsigned offset = 43;
    for (unsigned i = 0; i < parameters.getSize(); i++)
    {
        InParameter* inParam = dynamic_cast<InParameter*>(&parameters[i]);
        m_packet.setInt32(offset, ((inParam != 0) ? inParam->getInData().size() : 0) + 12);
        m_packet.setInt16(offset + 4, 0x1103);
        m_packet.setInt32(offset + 6, parameters[i].getSize());
        m_packet.setInt16(offset + 10, parameters[i].getType(dataStreamLevel));
        if (inParam != 0)
        {
            m_packet.setBytes(offset + 12, inParam->getInData());
            offset += inParam->getInData().size();
        }
        offset += 12;
    }
}

const char* CallProgramRequest::getName() const
{
    return "call program";
}

}
