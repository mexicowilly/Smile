#include "ServiceFactory.hpp"

namespace smile
{

ServiceFactory::ServiceFactory(AS400& as400)
    : m_as400(as400.getImplementation())
{
}

ServiceFactory::~ServiceFactory()
{
}

}
