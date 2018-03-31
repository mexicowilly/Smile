#if !defined(HOMINID_TESTOBSERVER_HPP__)
#define HOMINID_TESTOBSERVER_HPP__

#include <hominid/Test.hpp>

namespace hominid
{

class TestObserver
{
public:
    virtual ~TestObserver();
    virtual void errorAdded(const Test& test, const std::string& message);
    virtual void failureAdded(const Test& test, const std::string& message);
    virtual void testEnded(const Test& test);
    virtual void testStarted(const Test& test);
};

}

#endif
