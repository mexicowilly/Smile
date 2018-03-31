#include "Connection.hpp"
#include <smile/Text.hpp>
#include <smile/Exception.hpp>
#include "Locker.hpp"
#include <sstream>
#include <unistd.h>
#include <errno.h>
#include <sys/select.h>

namespace smile
{

Connection::Connection()
    : m_bio(0)
{
}

Connection::Connection(const Text& hostName, int port)
    : m_bio(0), m_hostName(hostName), m_port(port)
{
}

Connection::Connection(const Connection& connection)
    : m_bio(connection.m_bio), m_hostName(connection.m_hostName), m_port(connection.m_port)
{
}

Connection::~Connection()
{
    disconnect();
}

Connection& Connection::operator= (const Connection& connection)
{
    if (this != &connection)
    {
        disconnect();
        m_hostName = connection.m_hostName;
        m_port = connection.m_port;
        m_bio = connection.m_bio;
    }
    return *this;
}

bool Connection::operator< (const Connection& connection)
{
    return m_bio != 0 &&
           connection.m_bio != 0 &&
           BIO_get_fd(m_bio, 0) < BIO_get_fd(connection.m_bio, 0);
}

void Connection::connect()
{
    if (isConnected())
        return;
    std::ostringstream stream;
    stream << m_hostName << ":" << m_port;
    m_bio = BIO_new_connect(const_cast<char*>(stream.str().c_str()));
    if (m_bio == 0)
        throw OpenSslException(__FILE__, __LINE__, "Could not create connection");
    if (BIO_do_connect(m_bio) <= 0)
        throw OpenSslException(__FILE__, __LINE__, "Could not open connection");
}

void Connection::disconnect()
{
    if (isConnected())
    {
        close(BIO_get_fd(m_bio, 0));
        BIO_free_all(m_bio);
        m_bio = 0;
    }
}

bool Connection::isConnected() const
{
    return m_bio != 0 && BIO_get_fd(m_bio, 0) != -1;
}

void Connection::read(uint8_t* bytes, size_t num)
{
    while (num > 0)
    {
        int received = BIO_read(m_bio, bytes, num);
        if (received <= 0)
        {
            pthread_testcancel();
            if (!BIO_should_retry(m_bio))
                throw OpenSslException(__FILE__, __LINE__, "Could not read from socket");
        }
        else
        {
            num -= received;
            bytes += received;
        }
    }
}

bool Connection::waitForInput(Time timeOut)
{
    bool result = false;
    if (isConnected())
    {
        int myFd = BIO_get_fd(m_bio, 0);
        fd_set fdSet;
        FD_ZERO(&fdSet);
        FD_SET(myFd, &fdSet);
        struct timeval waitTime;
        waitTime.tv_sec = timeOut / 1000;
        waitTime.tv_usec = (timeOut % 1000) * 1000;
        int rc = select(myFd + 1, &fdSet, 0, 0, &waitTime);
        if (rc < 0)
        {
            throw ErrnoException(__FILE__, __LINE__,
                "Could not wait for data to read", errno);
        }
        result = rc > 0;
    }
    return result;
}

void Connection::write(const uint8_t* const bytes, size_t num)
{
    uint8_t* localBytes = const_cast<uint8_t*>(bytes);
    while (num > 0)
    {
        int written = BIO_write(m_bio, localBytes, num);
        if (written <= 0)
        {
            pthread_testcancel();
            if (!BIO_should_retry(m_bio))
                throw OpenSslException(__FILE__, __LINE__, "Could not write to socket");
        }
        else
        {
            num -= written;
            localBytes += written;
        }
    }
}

}
