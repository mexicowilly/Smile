#if !defined(SMILE_PROGRAM_HPP__)
#define SMILE_PROGRAM_HPP__

#include <smile/AS400.hpp>
#include <smile/OS400Message.hpp>
#include <smile/ProgramParameterList.hpp>
#include <smile/QsysObjectName.hpp>

namespace smile
{

class RemoteCommandClient;

class Program
{
public:
    Program(AS400& as400);
    Program(AS400& as400, const QsysObjectName& program);
    Program(AS400& as400,
            const QsysObjectName& program,
            const ProgramParameterList& parameters);
    Program(const Program& program);
    virtual ~Program();

    Program& operator= (const Program& program);

    int getCCSID() const;
    OS400Message::MessageCount getMessageCount() const;
    const std::vector<OS400Message>& getMessages() const;
    const ProgramParameterList& getParameters() const;
    const QsysObjectName& getProgram() const;
    virtual bool run();
    void setMessageCount(OS400Message::MessageCount messageCount);
    void setParameters(const ProgramParameterList& parameters);
    void setProgram(const QsysObjectName& program);

protected:
    RemoteCommandClient& getClient();
    ProgramParameterList& getMutableParameters();

private:
    QsysObjectName m_program;
    ProgramParameterList m_parameters;
    RemoteCommandClient* m_client;
    OS400Message::MessageCount m_messageCount;
};

inline RemoteCommandClient& Program::getClient()
{
    return *m_client;
}

inline OS400Message::MessageCount Program::getMessageCount() const
{
    return m_messageCount;
}

inline ProgramParameterList& Program::getMutableParameters()
{
    return m_parameters;
}

inline const ProgramParameterList& Program::getParameters() const
{
    return m_parameters;
}

inline const QsysObjectName& Program::getProgram() const
{
    return m_program;
}

inline void Program::setMessageCount(OS400Message::MessageCount messageCount)
{
    m_messageCount = messageCount;
}

inline void Program::setParameters(const ProgramParameterList& parameters)
{
    m_parameters = parameters;
}

inline void Program::setProgram(const QsysObjectName& program)
{
    m_program = program;
}

}

#endif
