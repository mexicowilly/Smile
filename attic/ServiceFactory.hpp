#if !defined(SMILE_SERVICEINITIALIZER_HPP__)
#define SMILE_SERVICEINITIALIZER_HPP__

#include "AS400Impl.hpp"

namespace smile
{

class HostService;

class ServiceFactory
{
public:
    virtual ~ServiceFactory();

    virtual HostService* createHostService() = 0;
    AS400& getAS400();
    virtual uint16_t getServiceIdentifier() = 0;

protected:
    ServiceFactory(AS400& as400);

    AS400Impl& m_as400;
};

inline AS400& ServiceFactory::getAS400()
{
    return m_as400.getAS400();
}

}

#endif
