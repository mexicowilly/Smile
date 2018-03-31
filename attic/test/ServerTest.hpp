#if !defined(SMILE_TEST_SERVERTEST_HPP__)
#define SMILE_TEST_SERVERTEST_HPP__

#include <smile/AS400.hpp>

class ServerTest
{
public:
    ServerTest();
    ServerTest(const ServerTest& st);
    ~ServerTest();

protected:
    smile::Text m_server;
    smile::Text m_user;
    smile::Text m_password;
    smile::AS400* m_as400;
};

#endif
