#include "smile/Command.hpp"
#include "RemoteCommandClient.hpp"

namespace smile
{

Command::Command(AS400& as400)
    : m_client(new RemoteCommandClient(as400)),
      m_messageCount(OS400Message::MessageCount_All)
{
}

Command::Command(AS400& as400, const Text& command)
    : m_command(command),
      m_client(new RemoteCommandClient(as400)),
      m_messageCount(OS400Message::MessageCount_All)
{
}

Command::Command(const Command& command)
    : m_command(command.m_command),
      m_client(new RemoteCommandClient(command.m_client->getAS400())),
      m_messageCount(command.m_messageCount)
{
}

Command::~Command()
{
    delete m_client;
}

Command& Command::operator= (const Command& command)
{
    if (&command != this)
    {
        m_command = command.m_command;
        delete m_client;
        m_client = new RemoteCommandClient(command.m_client->getAS400());
        m_messageCount = command.m_messageCount;
    }
    return *this;
}

const std::vector<OS400Message>& Command::getMessages() const
{
    return m_client->getMessages();
}

bool Command::run()
{
    return m_client->runCommand(m_command, m_messageCount);
}

}
