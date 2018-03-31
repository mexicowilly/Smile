#include "AS400ImplTest.hpp"
#include "AS400Impl.hpp"
#include "SignonService.hpp"
#include "RemoteCommandService.hpp"
#include <iostream>

void AS400ImplTest::setUp()
{
    m_localAS400 = new smile::AS400(m_server, m_user, m_password);
}

void AS400ImplTest::tearDown()
{
    delete m_localAS400;
    m_localAS400 = 0;
}

void AS400ImplTest::testGetCCSID()
{
    int ccsid;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, ccsid = m_localAS400->getImplementation().getCCSID());
    std::cout << "Got default CCSID from server of " << ccsid << std::endl;
}

void AS400ImplTest::testGetPort()
{
    smile::SignonService signonService(m_localAS400->getImplementation());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_localAS400->getImplementation().getPort(signonService));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_localAS400->getImplementation().getPort(signonService));
    smile::RemoteCommandService remoteCommandService(m_localAS400->getImplementation());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_localAS400->getImplementation().getPort(remoteCommandService));
}

void AS400ImplTest::testPrepareConnection()
{
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_localAS400->getImplementation().prepareConnection());
}

void AS400ImplTest::testSignon()
{
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_localAS400->getImplementation().signon(false));
}
