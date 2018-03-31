#include "SecureConnection.hpp"
#include "Locker.hpp"
#include <smile/Exception.hpp>
#include <sstream>

namespace
{

pthread_once_t openSslOnce = PTHREAD_ONCE_INIT;

#if defined(__SUNPRO_CC)
extern "C" void openSslInit();
#endif

void openSslInit()
{
    OpenSSL_add_all_algorithms();
}

}

namespace smile
{

void SecureConnection::connect()
{
    if (isConnected())
        return;
    pthread_once(&openSslOnce, openSslInit);
    m_sslContext = SSL_CTX_new(SSLv23_client_method());
    if (m_sslContext == 0)
        throw OpenSslException(__FILE__, __LINE__, "Could not create SSL context");
    m_bio = BIO_new_ssl_connect(m_sslContext);
    if (m_bio == 0)
        throw OpenSslException(__FILE__, __LINE__, "Could not create SSL connection");
    SSL* ssl;
    BIO_get_ssl(m_bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    std::ostringstream stream;
    stream << getHostName() << ":" << getPort();
    BIO_set_conn_hostname(m_bio, const_cast<char*>(stream.str().c_str()));
    if (BIO_do_connect(m_bio) <= 0)
        throw OpenSslException(__FILE__, __LINE__, "Could not open SSL connection");
}

void SecureConnection::disconnect()
{
    Connection::disconnect();
    if (m_sslContext != 0)
    {
        SSL_CTX_free(m_sslContext);
        m_sslContext = 0;
    }
}

}
