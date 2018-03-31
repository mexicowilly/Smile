#include "ServicePoolTest.hpp"
#include "ServicePool.hpp"
#include <smile/System.hpp>
#include <pthread.h>

namespace
{

class FakeHostService : public smile::HostService
{
public:
    FakeHostService(smile::AS400Impl& as400);

    virtual const char* getName() const;
    virtual uint16_t getIdentifier() const;
    virtual smile::PacketReply* receive(uint32_t correlation);
    virtual uint32_t send(smile::PacketRequest& request);

protected:
    virtual smile::PacketReply* convertPacketToReply(const smile::AccessPacket& packet);
};

class FakeServiceFactory : public smile::ServiceFactory
{
public:
    FakeServiceFactory(smile::AS400& as400);

    virtual smile::HostService* createHostService();
    virtual uint16_t getServiceIdentifier();
};

struct ThreadArg
{
    double wait;
    const char* name;
    smile::AS400* as400;
};

FakeHostService::FakeHostService(smile::AS400Impl& as400)
    : smile::HostService(as400)
{
}

const char* FakeHostService::getName() const
{
    return "fake";
}

uint16_t FakeHostService::getIdentifier() const
{
    return 11;
}

smile::PacketReply* FakeHostService::receive(uint32_t correlation)
{
    return 0;
}

uint32_t FakeHostService::send(smile::PacketRequest& request)
{
    return 0;
}

smile::PacketReply* FakeHostService::convertPacketToReply(const smile::AccessPacket& packet)
{
    return 0;
}

FakeServiceFactory::FakeServiceFactory(smile::AS400& as400)
    : smile::ServiceFactory(as400)
{
}

smile::HostService* FakeServiceFactory::createHostService()
{
    return new FakeHostService(m_as400);
}

uint16_t FakeServiceFactory::getServiceIdentifier()
{
    return 11;
}

void sleepyTime(double seconds)
{
    if (seconds > 0.0)
    {
        struct timespec timeSpec;
        timeSpec.tv_sec = static_cast<time_t>(seconds / 1.0);
        timeSpec.tv_nsec = static_cast<long>(1000000000.0 *
            (seconds - static_cast<double>(timeSpec.tv_sec)));
        nanosleep(&timeSpec, 0);
    }
}

#if defined(__SUNPRO_CC)
extern "C" {
void* overflowThreadMain(void* arg);
void* reuseThreadMain(void* arg);
}
#endif

void* overflowThreadMain(void* arg)
{
    ThreadArg* targ = reinterpret_cast<ThreadArg*>(arg);
    smile::ServicePool& pool = smile::ServicePool::getInstance();
    FakeServiceFactory factory(*targ->as400);
    smile::HostService* service = pool.checkOut(factory);
    sleepyTime(2.0);
    pool.checkIn(service);
    return 0;
}

void* reuseThreadMain(void* arg)
{
    ThreadArg* targ = reinterpret_cast<ThreadArg*>(arg);
    smile::ServicePool& pool = smile::ServicePool::getInstance();
    sleepyTime(targ->wait);
    FakeServiceFactory factory(*targ->as400);
    smile::HostService* service;
    for (int i = 0; i < 2; i++)
    {
        service = pool.checkOut(factory);
        sleepyTime(0.50);
        pool.checkIn(service);
        sleepyTime(0.50);
    }
    return 0;
}

}

void ServicePoolTest::setUp()
{
    m_originalState = smile::System::getInstance().getProperties().getBool("service.pool.enabled");
    if (!m_originalState)
        smile::System::getInstance().getProperties().setBool("service.pool.enabled", true);
}

void ServicePoolTest::tearDown()
{
    if (!m_originalState)
        smile::System::getInstance().getProperties().setBool("service.pool.enabled", false);
}

void ServicePoolTest::testMultiThreadOverflow()
{
    smile::Properties& sysProps = smile::System::getInstance().getProperties();
    int64_t capacity = sysProps.getInt("service.pool.capacity");
    int64_t wait = sysProps.getInt("service.pool.capacity");
    sysProps.setInt("service.pool.capacity", 1);
    sysProps.setInt("service.pool.wait.timeout", 1);
    smile::AS400 as400("overflow", "fleas", "has fleas");
    ThreadArg arg;
    arg.as400 = &as400;
    pthread_t thread;
    pthread_create(&thread, 0, overflowThreadMain, &arg);
    sleepyTime(0.25);
    smile::ServicePool& pool = smile::ServicePool::getInstance();
    FakeServiceFactory factory(as400);
    smile::HostService* service = pool.checkOut(factory);
    sleepyTime(0.5);
    pool.checkIn(service);
    pthread_join(thread, 0);
    sysProps.setInt("service.pool.capacity", capacity);
    sysProps.setInt("service.pool.wait.timeout", wait);
}

void ServicePoolTest::testMultiThreadReuse()
{
    smile::Properties& sysProps = smile::System::getInstance().getProperties();
    int64_t original = sysProps.getInt("service.pool.capacity");
    sysProps.setInt("service.pool.capacity", 1);
    smile::AS400 as400("my", "doggy", "has fleas");
    ThreadArg arg1;
    arg1.wait = 0.25;
    arg1.name = "thread2";
    arg1.as400 = &as400;
    pthread_t thread;
    pthread_create(&thread, 0, reuseThreadMain, &arg1);
    ThreadArg arg2;
    arg2.wait = 0.0;
    arg2.name = "thread1";
    arg2.as400 = &as400;
    reuseThreadMain(&arg2);
    pthread_join(thread, 0);
    sysProps.setInt("service.pool.capacity", original);
}

void ServicePoolTest::testSingleThreadOverflow()
{
    smile::Properties& sysProps = smile::System::getInstance().getProperties();
    int64_t original = sysProps.getInt("service.pool.capacity");
    sysProps.setInt("service.pool.capacity", 1);
    smile::HostService* services[2];
    smile::AS400 as400("my", "dog", "has fleas");
    FakeServiceFactory factory(as400);
    smile::ServicePool& pool = smile::ServicePool::getInstance();
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, services[0] = pool.checkOut(factory));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, services[1] = pool.checkOut(factory));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, pool.checkIn(services[0]));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, pool.checkIn(services[1]));
    sysProps.setInt("service.pool.capacity", original);
}

void ServicePoolTest::testSingleThreadReuse()
{
    smile::ServicePool& pool = smile::ServicePool::getInstance();
    smile::AS400 as400("my", "dog", "has fleas");
    FakeServiceFactory factory(as400);
    smile::HostService* service;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, service = pool.checkOut(factory));
    predicate(HERE, dynamic_cast<FakeHostService*>(service) != 0);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, pool.checkIn(service));
    smile::HostService* service2;
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, service2 = pool.checkOut(factory));
    predicateEquals(HERE, service, service2);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, pool.checkIn(service2));
}
