#if !defined(SMILE_OS400MESSAGE_HPP__)
#define SMILE_OS400MESSAGE_HPP__

#include <smile/Text.hpp>
#include <smile/Time.hpp>
#include <smile/QsysObjectName.hpp>
#include <smile/AS400.hpp>

namespace smile
{

class OS400Message
{
public:
    enum Type
    {
        Type_Unset                      = 0,
        Type_Completion                 = 1,
        Type_Diagnostic                 = 2,
        Type_Informational              = 4,
        Type_Inquiry                    = 5,
        Type_SendersCopy                = 6,
        Type_Request                    = 8,
        Type_RequestWithPrompting       = 10,
        Type_Notification               = 14,
        Type_Escape                     = 15,
        Type_NotificationNotHandled     = 16,
        Type_EscapeNotHandled           = 17,
        Type_ReplyNotValidityChecked    = 21,
        Type_ReplyValidityChecked       = 22,
        Type_ReplyMessageDefaultUsed    = 23,
        Type_ReplySystemDefaultUsed     = 24,
        Type_ReplyFromSystemReplyList   = 25
    };

    enum MessageCount
    {
        MessageCount_None       = 1,
        MessageCount_UpTo10     = 0,
        MessageCount_All        = 2
    };

    OS400Message(AS400& as400);
    OS400Message(const OS400Message& message);

    OS400Message& operator= (const OS400Message& message);

    AS400& getAS400() const;
    const Text& getDefaultReply() const;
    const Text& getHelp() const;
    const Text& getMessageIdentifier() const;
    const QsysObjectName& getMessageFile() const;
    unsigned getSeverity() const;
    const std::vector<uint8_t>& getSubstitutionData() const;
    const Text& getText() const;
    Time getTime() const;
    Type getType() const;
    void load();
    void setDefaultReply(const Text& defaultReply);
    void setHelp(const Text& help);
    void setMessageIdentifier(const Text& messageId);
    void setMessageFile(const QsysObjectName& messageFile);
    void setSeverity(unsigned severity);
    void setSubstitutionData(const std::vector<uint8_t>& substitutionData);
    void setText(const Text& text);
    void setTime(Time date);
    void setType(OS400Message::Type type);

private:
    Time m_time;
    Text m_messageId;
    Text m_defaultReply;
    std::vector<uint8_t> m_substitutionData;
    Text m_text;
    Text m_help;
    unsigned m_severity;
    Type m_type;
    QsysObjectName m_messageFile;
    AS400* m_as400;
};

inline AS400& OS400Message::getAS400() const
{
    return *m_as400;
}

inline const Text& OS400Message::getDefaultReply() const
{
    return m_defaultReply;
}

inline const Text& OS400Message::getHelp() const
{
    return m_help;
}

inline const Text& OS400Message::getMessageIdentifier() const
{
    return m_messageId;
}

inline const QsysObjectName& OS400Message::getMessageFile() const
{
    return m_messageFile;
}

inline unsigned OS400Message::getSeverity() const
{
    return m_severity;
}

inline const std::vector<uint8_t>& OS400Message::getSubstitutionData() const
{
    return m_substitutionData;
}

inline const Text& OS400Message::getText() const
{
    return m_text;
}

inline Time OS400Message::getTime() const
{
    return m_time;
}

inline OS400Message::Type OS400Message::getType() const
{
    return m_type;
}

inline void OS400Message::setTime(Time date)
{
    m_time = date;
}

inline void OS400Message::setDefaultReply(const Text& defaultReply)
{
    m_defaultReply = defaultReply;
}

inline void OS400Message::setHelp(const Text& help)
{
    m_help = help;
}

inline void OS400Message::setMessageIdentifier(const Text& messageId)
{
    m_messageId = messageId;
}

inline void OS400Message::setMessageFile(const QsysObjectName& messageFile)
{
    m_messageFile = messageFile;
}

inline void OS400Message::setSeverity(unsigned severity)
{
    m_severity = severity;
}

inline void OS400Message::setSubstitutionData(const std::vector<uint8_t>& substitutionData)
{
    m_substitutionData = substitutionData;
}

inline void OS400Message::setText(const Text& text)
{
    m_text = text;
}

inline void OS400Message::setType(Type type)
{
    m_type = type;
}

}

std::ostream& operator<< (std::ostream& stream, const smile::OS400Message& message);

#endif
