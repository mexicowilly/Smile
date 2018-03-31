#include "MessageReply.hpp"
#include <assert.h>

namespace smile
{

std::vector<OS400Message> MessageReply::getMessages(AS400& as400) const
{
    assert(m_ccsid != -1);
    std::vector<OS400Message> output;
    size_t messageCount = m_packet.getInt16(22);
    unsigned offset = 24;
    for (size_t i = 0; i < messageCount; i++)
    {
        OS400Message message(as400);
        if (m_packet.getInt8(offset + 5) == 6)
        {
            size_t localOffset = offset;
            int textCCSID = m_packet.getInt32(localOffset);
            localOffset += 4;
            // int substCCSID = m_packet.getInt32(localOffset);
            localOffset += 4;
            message.setSeverity(m_packet.getInt16(localOffset));
            localOffset += 2;
            size_t typeLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            message.setType(static_cast<OS400Message::Type>(((m_packet.getInt8(localOffset) & 0x0f) * 10) +
                            (m_packet.getInt8(localOffset + 1) & 0x0f)));
            localOffset += typeLength;
            size_t idLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            std::vector<uint8_t> bytes = m_packet.getBytes(localOffset, idLength);
            localOffset += idLength;
            message.setMessageIdentifier(Text(&bytes[0], bytes.size(), m_ccsid));
            size_t fileNameLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            bytes = m_packet.getBytes(localOffset, fileNameLength);
            localOffset += fileNameLength;
            Text messageFileName(&bytes[0], bytes.size(), m_ccsid);
            messageFileName.trim();
            size_t libraryLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            bytes = m_packet.getBytes(localOffset, libraryLength);
            localOffset += libraryLength;
            Text messageFileLibrary(&bytes[0], bytes.size(), m_ccsid);
            messageFileLibrary.trim();
            message.setMessageFile(QsysObjectName(messageFileLibrary, messageFileName));
            size_t textLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            bytes = m_packet.getBytes(localOffset, textLength);
            localOffset += textLength;
            message.setText(Text(&bytes[0], bytes.size(), textCCSID));
            size_t substLength = m_packet.getInt32(localOffset);
            localOffset += 4;
            message.setSubstitutionData(m_packet.getBytes(localOffset, substLength));
            localOffset += substLength;
            size_t helpLength = m_packet.getInt32(localOffset);
            bytes = m_packet.getBytes(localOffset, helpLength);
            message.setHelp(Text(&bytes[0], bytes.size(), m_ccsid));
        }
        else
        {
            std::vector<uint8_t> bytes = m_packet.getBytes(offset + 6, 7);
            message.setMessageIdentifier(Text(&bytes[0], bytes.size(), m_ccsid));
            message.setType(static_cast<OS400Message::Type>(((m_packet.getInt8(offset + 13) & 0x0f) * 10) +
                            (m_packet.getInt8(offset + 14) & 0x0f)));
            message.setSeverity(m_packet.getInt16(15));
            bytes = m_packet.getBytes(offset + 17, 10);
            Text messageFileName(&bytes[0], bytes.size(), m_ccsid);
            messageFileName.trim();
            bytes = m_packet.getBytes(offset + 27, 10);
            Text messageFileLibrary(&bytes[0], bytes.size(), m_ccsid);
            messageFileLibrary.trim();
            message.setMessageFile(QsysObjectName(messageFileName, messageFileLibrary));
            uint16_t substitutionDataLength = m_packet.getInt16(offset + 37);
            message.setSubstitutionData(m_packet.getBytes(offset + 41, substitutionDataLength));
            bytes = m_packet.getBytes(offset + 41 + substitutionDataLength, m_packet.getInt16(offset + 39));
            message.setText(Text(&bytes[0], bytes.size(), m_ccsid));
        }
        output.push_back(message);
        offset += m_packet.getInt32(offset);
   }
   return output;
}

}
