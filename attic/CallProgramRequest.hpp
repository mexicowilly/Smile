#if !defined(SMILE_CALLPROGRAMREQUEST_HPP__)
#define SMILE_CALLPROGRAMREQUEST_HPP__

#include "PacketRequest.hpp"
#include "smile/ProgramParameterList.hpp"
#include "smile/OS400Message.hpp"

namespace smile
{

class CallProgramRequest : public PacketRequest
{
public:
    CallProgramRequest(const std::vector<uint8_t>& program,
                       ProgramParameterList& parameters,
                       uint16_t dataStreamLevel,
                       OS400Message::MessageCount messageCount);

    virtual const char* getName() const;
};

}

#endif
