#include "ServiceProgramProcedureTest.hpp"
#include <smile/ServiceProgramProcedure.hpp>
#include "QlgPathName.hpp"
#include "ByteOrder.hpp"
#include <iostream>

void ServiceProgramProcedureTest::testQp0lGetAttr()
{
    smile::QsysObjectName serviceProgram("QSYS", "QP0LLIB2");
    smile::Text procedureName("Qp0lGetAttr");
    smile::ProgramParameterList parameters;
    parameters.addParameter(
        smile::QlgPathName(smile::IfsObjectName("/QOpenSys/bin"),
                           smile::ProgramParameter::PassBy_Reference));
    std::vector<uint8_t> attrTypes(8, 0);
    attrTypes[3] = 1;
    parameters.addParameter(
        smile::InParameter(attrTypes, smile::ProgramParameter::PassBy_Reference));
    parameters.addParameter(
        smile::OutParameter(26, smile::ProgramParameter::PassBy_Reference));
    parameters.addParameter(
        smile::Int32InParameter(26, smile::ProgramParameter::PassBy_Value));
    parameters.addParameter(
        smile::Int32OutParameter(smile::ProgramParameter::PassBy_Reference));
    parameters.addParameter(
        smile::Int32OutParameter(smile::ProgramParameter::PassBy_Reference));
    parameters.addParameter(
        smile::Int32InParameter(1, smile::ProgramParameter::PassBy_Value));
    smile::ServiceProgramProcedure procedure(*m_as400, serviceProgram, procedureName, parameters);
    procedure.setReturnType(smile::ServiceProgramProcedure::ReturnType_Integer);
    bool result = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = procedure.run());
    const std::vector<smile::OS400Message>& messages = procedure.getMessages();
    if (messages.size() > 0)
        std::cout << std::endl;
    for (std::vector<smile::OS400Message>::const_iterator itor = messages.begin();
         itor != messages.end();
         itor++)
    {
        std::cout << *itor << std::endl;
    }
    std::cout << "Return code: " << procedure.getReturnCode() << std::endl;
    std::cout << "Error code: " << procedure.getErrorCode() << std::endl;
    predicate(HERE, result);
    predicateEquals(HERE, 0, procedure.getReturnCode());
}

void ServiceProgramProcedureTest::testGettimeofday()
{
    smile::QsysObjectName serviceProgram("QSYS", "QP0ZCPA");
    smile::Text procedureName("Qp0zGetTimeofDay");
    smile::ProgramParameterList parameters;
    parameters.addParameter(
        smile::OutParameter(8, smile::ProgramParameter::PassBy_Reference));
    parameters.addParameter(
        smile::OutParameter(8, smile::ProgramParameter::PassBy_Reference));
    smile::ServiceProgramProcedure procedure(*m_as400, serviceProgram, procedureName, parameters);
    procedure.setReturnType(smile::ServiceProgramProcedure::ReturnType_Integer);
    bool result = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = procedure.run());
    const std::vector<smile::OS400Message>& messages = procedure.getMessages();
    if (messages.size() > 0)
        std::cout << std::endl;
    for (std::vector<smile::OS400Message>::const_iterator itor = messages.begin();
         itor != messages.end();
         itor++)
    {
        std::cout << *itor << std::endl;
    }
    std::cout << "Return code: " << procedure.getReturnCode() << std::endl;
    std::cout << "Error code: " << procedure.getErrorCode() << std::endl;
    predicate(HERE, result);
    predicateEquals(HERE, 0, procedure.getReturnCode());
}
