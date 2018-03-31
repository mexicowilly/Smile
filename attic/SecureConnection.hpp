#if !defined(SMILE_SECURECONNECTION_HPP__)
#define SMILE_SECURECONNECTION_HPP__

#include "Connection.hpp"
#include <openssl/ssl.h>

namespace smile
{

class SecureConnection : public Connection
{
public:
    virtual void connect();
    virtual void disconnect();

private:
    SSL_CTX* m_sslContext;
};

}

#endif
