#include "CentralClient.hpp"
#include "NlsExchangeAttributesRequest.hpp"
#include "NlsExchangeAttributesReply.hpp"
#include "ServicePool.hpp"

namespace smile
{

CentralClient::CentralClient(AS400& as400)
    : ServiceFactory(as400),
      m_centralService(0)
{
}

CentralClient::~CentralClient()
{
    ServicePool::getInstance().checkIn(m_centralService);
}

void CentralClient::connect()
{
    if (m_centralService == 0)
        m_centralService = dynamic_cast<CentralService*>(ServicePool::getInstance().checkOut(*this));
}

HostService* CentralClient::createHostService()
{
    CentralService* central = new CentralService(m_as400);
    try
    {
        central->connect();
        NlsExchangeAttributesRequest request;
        NlsExchangeAttributesReply* reply;
        reply = central->sendAndReceive<NlsExchangeAttributesReply>(request);
        central->imbueAttributes(*reply);
        delete reply;
    }
    catch (...)
    {
        delete central;
        throw;
    }
    return central;
}

int CentralClient::getCCSID()
{
    connect();
    return m_centralService->getCCSID();
}

uint16_t CentralClient::getServiceIdentifier()
{
    return CentralService::IDENTIFIER;
}

}
