#include "CommandTest.hpp"
#include <smile/Command.hpp>
#include <iostream>

void CommandTest::testCommand()
{
    smile::Text cl("CRTPF FILE(QTEMP/TESTF) RCDLEN(72)");
    smile::Command command(*m_as400, cl);
    predicateEquals(HERE, cl, command.getCommand());
    command.setCommand("Chunky monkey");
    predicate(HERE, command.getCommand() != cl);
    command.setCommand(cl);
    predicateEquals(HERE, cl, command.getCommand());
    predicateEquals(HERE, smile::OS400Message::MessageCount_All, command.getMessageCount());
    command.setMessageCount(smile::OS400Message::MessageCount_None);
    predicateEquals(HERE, smile::OS400Message::MessageCount_None, command.getMessageCount());
    command.setMessageCount(smile::OS400Message::MessageCount_All);
    predicateEquals(HERE, smile::OS400Message::MessageCount_All, command.getMessageCount());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, command.run());
    const std::vector<smile::OS400Message>& messages = command.getMessages();
    predicate(HERE, messages.size() != 0);
    if (messages.size() > 0)
        std::cout << std::endl;
    for (std::vector<smile::OS400Message>::const_iterator itor = messages.begin();
         itor != messages.end();
         itor++)
    {
        std::cout << *itor << std::endl;
    }
}
