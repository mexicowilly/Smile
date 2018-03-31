#include "smile/ServiceProgramProcedure.hpp"
#include "RemoteCommandClient.hpp"

namespace smile
{

ServiceProgramProcedure::ServiceProgramProcedure(AS400& as400)
    : Program(as400),
      m_errorCode(0),
      m_returnCode(0),
      m_returnType(ReturnType_Void),
      m_emptyReturnIsSuccess(false)
{
}

ServiceProgramProcedure::ServiceProgramProcedure(AS400& as400,
                                                 const QsysObjectName& serviceProgram,
                                                 const Text& procedure)
    : Program(as400, serviceProgram),
      m_procedure(procedure),
      m_errorCode(0),
      m_returnCode(0),
      m_returnType(ReturnType_Void),
      m_emptyReturnIsSuccess(false)
{
}

ServiceProgramProcedure::ServiceProgramProcedure(AS400& as400,
                                                 const QsysObjectName& serviceProgram,
                                                 const Text& procedure,
                                                 const ProgramParameterList& parameters)
    : Program(as400, serviceProgram, parameters),
      m_procedure(procedure),
      m_errorCode(0),
      m_returnCode(0),
      m_returnType(ReturnType_Void),
      m_emptyReturnIsSuccess(false)
{
}

ServiceProgramProcedure::ServiceProgramProcedure(const ServiceProgramProcedure& procedure)
    : Program(procedure),
      m_procedure(procedure.m_procedure),
      m_errorCode(procedure.m_errorCode),
      m_returnCode(procedure.m_returnCode),
      m_returnType(procedure.m_returnType),
      m_emptyReturnIsSuccess(procedure.m_emptyReturnIsSuccess)
{
}

ServiceProgramProcedure& ServiceProgramProcedure::operator= (const ServiceProgramProcedure& procedure)
{
    if (&procedure != this)
    {
        Program::operator=(procedure);
        m_procedure = procedure.m_procedure;
        m_errorCode = procedure.m_errorCode;
        m_returnCode = procedure.m_returnCode;
        m_returnType = procedure.m_returnType;
        m_emptyReturnIsSuccess = procedure.m_emptyReturnIsSuccess;
    }
    return *this;
}

const std::vector<OS400Message>& ServiceProgramProcedure::getMessages() const
{
    return const_cast<ServiceProgramProcedure*>(this)->getClient().getMessages();
}

bool ServiceProgramProcedure::run()
{
    return getClient().callServiceProgramProcedure(getProgram(),
                                                   m_procedure,
                                                   getMutableParameters(),
                                                   getMessageCount(),
                                                   m_returnType,
                                                   m_returnCode,
                                                   m_errorCode,
                                                   m_emptyReturnIsSuccess);
}

}
