#include <smile/OS400Message.hpp>
#include <smile/MessageFile.hpp>
#include <smile/Exception.hpp>

std::ostream& operator<< (std::ostream& stream, const smile::OS400Message& message)
{
    stream << message.getMessageIdentifier() << ": " << message.getText();
    return stream;
}

namespace smile
{

OS400Message::OS400Message(AS400& as400)
    : m_time(0), m_severity(0), m_type(Type_Unset), m_as400(&as400)
{
}

OS400Message::OS400Message(const OS400Message& message)
    : m_time(message.m_time),
      m_messageId(message.m_messageId),
      m_defaultReply(message.m_defaultReply),
      m_substitutionData(message.m_substitutionData),
      m_text(message.m_text),
      m_help(message.m_help),
      m_severity(message.m_severity),
      m_type(message.m_type),
      m_messageFile(message.m_messageFile),
      m_as400(message.m_as400)
{
}

OS400Message& OS400Message::operator= (const OS400Message& message)
{
    if (&message != this)
    {
        m_time = message.m_time;
        m_messageId = message.m_messageId;
        m_defaultReply = message.m_defaultReply;
        m_substitutionData = message.m_substitutionData;
        m_text = message.m_text;
        m_help = message.m_help;
        m_severity = message.m_severity;
        m_type = message.m_type;
        m_messageFile = message.m_messageFile;
        m_as400 = message.m_as400;
    }
    return *this;
}

void OS400Message::load()
{
    if (m_messageFile.isEmpty())
        throw InvalidStateException(__FILE__, __LINE__, "The message file name has not been set");
    if (m_messageId.isEmpty())
        throw InvalidStateException(__FILE__, __LINE__, "The message identifier has not been set");
    MessageFile messageFile(*m_as400, m_messageFile);
    OS400Message msg = messageFile.getMessage(m_messageId, m_substitutionData);
    m_defaultReply = msg.m_defaultReply;
    m_text = msg.m_text;
    m_help = msg.m_help;
    m_severity = msg.m_severity;
}

}
