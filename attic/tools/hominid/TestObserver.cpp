#include <hominid/TestObserver.hpp>

namespace hominid
{

TestObserver::~TestObserver()
{
}

void TestObserver::errorAdded(const Test&, const std::string&)
{
}

void TestObserver::failureAdded(const Test&, const std::string&)
{
}

void TestObserver::testEnded(const Test&)
{
}

void TestObserver::testStarted(const Test&)
{
}

}
