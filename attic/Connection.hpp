#if !defined(SMILE_CONNECTION_HPP__)
#define SMILE_CONNECTION_HPP__

#include <smile/Time.hpp>
#include <inttypes.h>
#include <smile/Text.hpp>
#include <openssl/bio.h>

namespace smile
{

class Connection
{
public:
    Connection();
    Connection(const Text& hostName, int port);
    Connection(const Connection& connection);
    virtual ~Connection();

    Connection& operator= (const Connection& connection);
    bool operator== (const Connection& connection);
    bool operator!= (const Connection& connection);
    bool operator< (const Connection& connection);

    virtual void connect();
    virtual void disconnect();
    const Text& getHostName() const;
    int getPort() const;
    bool isConnected() const;
    virtual void read(uint8_t* bytes, size_t num);
    bool waitForInput(Time timeOut);
    virtual void write(const uint8_t* const bytes, size_t num);

protected:
    BIO* m_bio;

private:
    Text m_hostName;
    int m_port;
};

inline bool Connection::operator== (const Connection& connection)
{
    return m_bio == connection.m_bio;
}

inline bool Connection::operator!= (const Connection& connection)
{
    return !(*this == connection);
}

inline const Text& Connection::getHostName() const
{
    return m_hostName;
}

inline int Connection::getPort() const
{
    return m_port;
}

}

#endif
