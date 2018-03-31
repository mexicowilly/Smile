#if !defined(SMILE_COMMAND_HPP__)
#define SMILE_COMMAND_HPP__

#include <smile/AS400.hpp>
#include <smile/OS400Message.hpp>

namespace smile
{

class RemoteCommandClient;

class Command
{
public:
    Command(AS400& as400);
    Command(AS400& as400, const Text& command);
    Command(const Command& command);
    ~Command();

    Command& operator= (const Command& command);

    const Text& getCommand() const;
    OS400Message::MessageCount getMessageCount() const;
    const std::vector<OS400Message>& getMessages() const;
    bool run();
    void setCommand(const Text& command);
    void setMessageCount(OS400Message::MessageCount messageCount);

private:
    Text m_command;
    RemoteCommandClient* m_client;
    OS400Message::MessageCount m_messageCount;
};

inline const Text& Command::getCommand() const
{
    return m_command;
}

inline OS400Message::MessageCount Command::getMessageCount() const
{
    return m_messageCount;
}

inline void Command::setCommand(const Text& command)
{
    m_command = command;
}

inline void Command::setMessageCount(OS400Message::MessageCount messageCount)
{
    m_messageCount = messageCount;
}

}

#endif
