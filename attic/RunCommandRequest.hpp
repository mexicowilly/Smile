#if !defined(SMILE_RUNCOMMANDREQUEST_HPP__)
#define SMILE_RUNCOMMANDREQUEST_HPP__

#include "PacketRequest.hpp"
#include "smile/OS400Message.hpp"

namespace smile
{

class RunCommandRequest : public PacketRequest
{
public:
    RunCommandRequest(const std::vector<uint8_t>& command,
                      uint16_t dataStreamLevel,
                      OS400Message::MessageCount messageCount,
                      int ccsid);

    virtual const char* getName() const;
};

}

#endif
