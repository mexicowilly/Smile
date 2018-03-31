#include "ServerTest.hpp"
#include "TestEnvironment.hpp"

ServerTest::ServerTest()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    m_server = props.get("smile.test.as400").c_str();
    m_user = props.get("smile.test.user").c_str();
    m_password = props.get("smile.test.password").c_str();
    m_as400 = new smile::AS400(m_server, m_user, m_password);
}

ServerTest::ServerTest(const ServerTest& st)
    : m_server(st.m_server),
      m_user(st.m_user),
      m_password(st.m_password),
      m_as400(new smile::AS400(m_server, m_user, m_password))
{
}

ServerTest::~ServerTest()
{
    delete m_as400;
}
