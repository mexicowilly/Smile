#include <hominid/TestRunner.hpp>
#include "TextConverterTest.hpp"
#include "TextTest.hpp"
#include "OS400VersionTest.hpp"
#include "PacketTest.hpp"
#include "ConnectionTest.hpp"
#include "AS400ImplTest.hpp"
#include "CommandTest.hpp"
#include "ProgramTest.hpp"
#include "ServiceProgramProcedureTest.hpp"
#include "MessageFileTest.hpp"
#include "IfsFileTest.hpp"
#include "IfsObjectNameTest.hpp"
#include "PropertiesTest.hpp"
#include "NotifierTest.hpp"
#include "ServicePoolTest.hpp"
#include "IfsDirectoryTest.hpp"
#include "IfsFileHandleTest.hpp"
#include <smile/Exception.hpp>
#include <smile/Logger.hpp>
#include <smile/ConsoleLogWriter.hpp>
#include <iostream>
#include <algorithm>

int main(int argc, char* argv[])
{
    smile::Logger& log = smile::Logger::getLogger("smile");
    log.addLogWriter(smile::ConsoleLogWriter());
    log.setLogLevel(smile::LogLevel_All);

    hominid::TestRunner runner(std::cout);
    hominid::TestSuite suite("Smile Unit Tests");

    suite.addTest(TextTest::getTestSuite());
    suite.addTest(TextConverterTest::getTestSuite());
    suite.addTest(OS400VersionTest::getTestSuite());
    suite.addTest(PacketTest::getTestSuite());
    suite.addTest(ConnectionTest::getTestSuite());
    suite.addTest(AS400ImplTest::getTestSuite());
    suite.addTest(CommandTest::getTestSuite());
    suite.addTest(ProgramTest::getTestSuite());
    suite.addTest(ServiceProgramProcedureTest::getTestSuite());
    suite.addTest(MessageFileTest::getTestSuite());
    suite.addTest(IfsFileTest::getTestSuite());
    suite.addTest(IfsObjectNameTest::getTestSuite());
    suite.addTest(PropertiesTest::getTestSuite());
    suite.addTest(NotifierTest::getTestSuite());
    suite.addTest(ServicePoolTest::getTestSuite());
    suite.addTest(IfsDirectoryTest::getTestSuite());
    suite.addTest(IfsFileHandleTest::getTestSuite());

    if (argc > 1)
    {
        std::vector<std::string> tests;
        for(int i = 1; i < argc; i++)
        {
            std::string cpy(argv[i]);
            for (char* name = strtok(const_cast<char*>(cpy.c_str()), ", ");
                 name != 0;
                 name = strtok(0, ", "))
            {
                tests.push_back(name);
            }
        }
        if (std::find(tests.begin(), tests.end(), "all") == tests.end())
            suite.filter(tests);
    }

    try
    {
        runner.run(suite);
    }
    catch (smile::Exception& se)
    {
        std::cerr << "Uncaught smile exception:" << std::endl;
        std::cerr << se << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
