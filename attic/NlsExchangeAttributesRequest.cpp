#include "NlsExchangeAttributesRequest.hpp"
#include "CentralService.hpp"

namespace smile
{

NlsExchangeAttributesRequest::NlsExchangeAttributesRequest()
    : PacketRequest(38)
{
    m_packet.setServerId(CentralService::IDENTIFIER);
    m_packet.setTemplateLength(18);
    m_packet.setRequestReplyId(0x1301);
    m_packet.setInt32(24, 13488);
    m_packet.setInt32(28, 0x0310);
}

const char* NlsExchangeAttributesRequest::getName() const
{
    return "NLS exchange attributes";
}

}
