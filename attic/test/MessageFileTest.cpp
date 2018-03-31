#include "MessageFileTest.hpp"
#include <smile/MessageFile.hpp>
#include <iostream>

void MessageFileTest::testBareMessage()
{
    smile::QsysObjectName fileName("QSYS", "QCPFMSG");
    smile::MessageFile mf(*m_as400, fileName);
    smile::OS400Message msg(*m_as400);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, msg = mf.getMessage("CPF0006"));
    std::cout << "File: " << msg.getMessageFile().toDisplayName() << std::endl;
    std::cout << "Identifier: " << msg.getMessageIdentifier() << std::endl;
    std::cout << "Severity: " << msg.getSeverity() << std::endl;
    std::cout << "Text: " << msg.getText() << std::endl;
    std::cout << "Default reply: " << msg.getDefaultReply() << std::endl;
    std::cout << "Help: " << msg.getHelp() << std::endl;
}
