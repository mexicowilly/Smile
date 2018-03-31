#include "ConnectionTest.hpp"
#include "Connection.hpp"
#include <smile/Exception.hpp>

void ConnectionTest::testConnect()
{
    smile::Connection connection("www.ibm.com", 80);
    connection.connect();
    predicate(HERE, connection.isConnected());
    connection = smile::Connection("", 2222);
    predicate(HERE, !connection.isConnected());
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__, connection.connect(), smile::Exception);
}
