#include "SignonEndServerRequest.hpp"
#include "SignonService.hpp"

namespace smile
{

SignonEndServerRequest::SignonEndServerRequest()
    : PacketRequest(20)
{
    m_packet.setServerId(SignonService::IDENTIFIER);
    m_packet.setRequestReplyId(0x7006);
}

const char* SignonEndServerRequest::getName() const
{
    return "sign-on end server";
}

}
