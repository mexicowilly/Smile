#include "RemoteCommandExchangeAttributesRequest.hpp"
#include "smile/TextConverter.hpp"
#include "NationalLanguageVersion.hpp"
#include "LogUtil.hpp"
#include "RemoteCommandService.hpp"
#include <sstream>
#include <assert.h>

namespace smile
{

RemoteCommandExchangeAttributesRequest::RemoteCommandExchangeAttributesRequest()
    : PacketRequest(34)
{
    m_packet.setServerId(RemoteCommandService::IDENTIFIER);
    m_packet.setTemplateLength(14);
    m_packet.setRequestReplyId(0x1001);
    Logger& log = LogUtil::smileLogger();
    int defaultCCSID = TextConverter::getDefaultCCSID();
    SMILE_LOG_DEBUG(log, "Default CCSID: " << defaultCCSID);
    m_packet.setInt32(20, defaultCCSID);
    int defaultNLV = NationalLanguageVersion::getInstance().getDefaultCode();
    SMILE_LOG_DEBUG(log, "Default national language version code: " << defaultNLV);
    std::ostringstream stream;
    stream << defaultNLV;
    TextConverter defaultToAscii(SMILE_DEFAULT_CONVERTER, "US-ASCII");
    std::vector<uint8_t> ascii = defaultToAscii.convert(
        reinterpret_cast<const uint8_t*>(stream.str().c_str()));
    assert(ascii.size() == 4);
    m_packet.setInt8(24, ascii[0] | 0xf0);
    m_packet.setInt8(25, ascii[1] | 0xf0);
    m_packet.setInt8(26, ascii[2] | 0xf0);
    m_packet.setInt8(27, ascii[3] | 0xf0);
    m_packet.setInt32(28, 1);
}

const char* RemoteCommandExchangeAttributesRequest::getName() const
{
    return "remote command exchange attributes";
}

}
