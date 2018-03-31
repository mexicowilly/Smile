#if !defined(SMILE_CENTRALCLIENT_HPP__)
#define SMILE_CENTRALCLIENT_HPP__

#include "CentralService.hpp"
#include "ServiceFactory.hpp"

namespace smile
{

class CentralClient : public ServiceFactory
{
public:
    CentralClient(AS400& as400);
    ~CentralClient();

    virtual HostService* createHostService();
    int getCCSID();
    virtual uint16_t getServiceIdentifier();

private:
    void connect();

    CentralService* m_centralService;
};

}

#endif
