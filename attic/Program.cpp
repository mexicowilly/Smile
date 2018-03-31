#include "smile/Program.hpp"
#include "RemoteCommandClient.hpp"

namespace smile
{

Program::Program(AS400& as400)
    : m_client(new RemoteCommandClient(as400)),
      m_messageCount(OS400Message::MessageCount_All)
{
}

Program::Program(AS400& as400, const QsysObjectName& program)
    : m_program(program),
      m_client(new RemoteCommandClient(as400)),
      m_messageCount(OS400Message::MessageCount_All)
{
}

Program::Program(AS400& as400,
                 const QsysObjectName& program,
                 const ProgramParameterList& parameters)
    : m_program(program),
      m_parameters(parameters),
      m_client(new RemoteCommandClient(as400)),
      m_messageCount(OS400Message::MessageCount_All)
{
}

Program::Program(const Program& program)
    : m_program(program.m_program),
      m_parameters(program.m_parameters),
      m_client(new RemoteCommandClient(program.m_client->getAS400())),
      m_messageCount(program.m_messageCount)
{
}

Program::~Program()
{
    delete m_client;
}

Program& Program::operator= (const Program& program)
{
    if (&program != this)
    {
        m_program = program.m_program;
        m_parameters = program.m_parameters;
        delete m_client;
        m_client = new RemoteCommandClient(program.m_client->getAS400());
        m_messageCount = program.m_messageCount;
    }
    return *this;
}

int Program::getCCSID() const
{
    return m_client->getCCSID();
}

const std::vector<OS400Message>& Program::getMessages() const
{
    return m_client->getMessages();
}

bool Program::run()
{
    return m_client->callProgram(m_program, m_parameters, m_messageCount);
}

}
