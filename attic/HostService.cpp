#include "HostService.hpp"
#include "Locker.hpp"

namespace smile
{

HostService::HostService(AS400Impl& as400)
    : m_as400(as400), m_lastCorrelation(0)
{
    pthread_mutex_init(&m_correlationMutex, 0);
    pthread_mutex_init(&m_discardMutex, 0);
}

HostService::~HostService()
{
    pthread_mutex_destroy(&m_correlationMutex);
    pthread_mutex_destroy(&m_discardMutex);
}

void HostService::connect()
{
    if (!m_connection.isConnected())
    {
        m_connection = Connection(m_as400.getSystemName(), m_as400.getPort(*this));
        m_connection.connect();
    }
}

void HostService::disconnect()
{
    m_connection.disconnect();
}

bool HostService::isConnected() const
{
    return m_connection.isConnected();
}

uint32_t HostService::nextCorrelationId()
{
    Locker locker(m_correlationMutex);
    if (++m_lastCorrelation == 0)
        ++m_lastCorrelation;
    uint32_t saved = m_lastCorrelation;
    return saved;
}

void HostService::sendIgnoringReply(PacketRequest& request)
{
    uint32_t correlation = send(request);
    Locker locker(m_discardMutex);
    m_discarded.insert(correlation);
}

}
