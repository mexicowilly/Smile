#include "ProgramTest.hpp"
#include <smile/Program.hpp>
#include <arpa/inet.h>
#include <iostream>

void ProgramTest::testProgram()
{
    smile::QsysObjectName programName("QSYS", "QUSROBJD");
    smile::ProgramParameterList parameters;
    parameters.addParameter(smile::OutParameter(90));
    parameters.addParameter(smile::Int32InParameter(90));
    parameters.addParameter(smile::TextInParameter("OBJD0100"));
    parameters.addParameter(smile::QsysObjectNameInParameter(smile::QsysObjectName("QSYS", "QTEMP")));
    parameters.addParameter(smile::TextInParameter("*LIB      "));
    smile::Program program(*m_as400, programName, parameters);
    bool result = false;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, result = program.run());
    predicate(HERE, result);
    const std::vector<uint8_t>& objd = dynamic_cast<const smile::OutParameter&>(program.getParameters()[0]).getOutData();
    predicateEquals(HERE, static_cast<std::vector<uint8_t>::size_type>(90), objd.size());
    std::cout << "Got following description information for QSYS/QTEMP:" << std::endl;
    std::cout << "  Actual bytes returned: " << objd.size() << std::endl;
    std::cout << "  Reported bytes returned: " << ntohl(*reinterpret_cast<const uint32_t*>(&objd[0])) << std::endl;
    std::cout << "  Object name: " << smile::Text(&objd[8], 10, program.getCCSID()) << std::endl;
    std::cout << "  Object library: " << smile::Text(&objd[18], 10, program.getCCSID()) << std::endl;
    std::cout << "  Object type: " << smile::Text(&objd[28], 10, program.getCCSID()) << std::endl;
    std::cout << "  Object return library: " << smile::Text(&objd[38], 10, program.getCCSID()) << std::endl;
    std::cout << "  ASP number: " << ntohl(*reinterpret_cast<const uint32_t*>(&objd[48])) << std::endl;
    std::cout << "  Object owner: " << smile::Text(&objd[52], 10, program.getCCSID()) << std::endl;
    std::cout << "  Object domain: " << smile::Text(&objd[62], 2, program.getCCSID()) << std::endl;
    std::cout << "  Create time: " << smile::Text(&objd[64], 13, program.getCCSID()) << std::endl;
    std::cout << "  Change time: " << smile::Text(&objd[77], 13, program.getCCSID()) << std::endl;
}
