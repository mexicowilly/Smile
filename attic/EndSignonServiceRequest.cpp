#include "EndSignonServiceRequest.hpp"
#include "SignonService.hpp"

namespace smile
{

EndSignonServiceRequest::EndSignonServiceRequest()
    : PacketRequest(20)
{
    m_packet.setServerId(SignonService::IDENTIFIER);
    m_packet.setRequestReplyId(0x7006);
}

const char* EndSignonServiceRequest::getName() const
{
    return "end sign-on service";
}

}
