#include "CallProgramReply.hpp"
#include "RunLengthEncoder.hpp"
#include "LogUtil.hpp"

namespace smile
{

const char* CallProgramReply::getName() const
{
    return "call program";
}

void CallProgramReply::imbueOutputParameters(ProgramParameterList& parameters)
{
    unsigned offset = 24;
    Logger& log = LogUtil::smileLogger();
    for (unsigned i = 0; i < parameters.getSize(); i++)
    {
        OutParameter* outParam = dynamic_cast<OutParameter*>(&parameters[i]);
        if (outParam != 0)
        {
            uint32_t totalLength = m_packet.getInt32(offset);
            if (totalLength > 12)
            {
                uint16_t usage = m_packet.getInt16(offset + 10);
                std::vector<uint8_t> outputData;
                if (usage == 22 || usage == 23)
                    outputData = RunLengthEncoder::decode(m_packet.getBytes(offset + 12, totalLength - 12));
                else
                    outputData = m_packet.getBytes(offset + 12, totalLength - 12);
                outParam->setOutData(outputData);
                SMILE_LOG_DEBUG(log, "Parameter " << i << "receiving: " <<
                    LogUtil::bytes(outputData));
            }
            offset += totalLength;
        }
    }
}

}
