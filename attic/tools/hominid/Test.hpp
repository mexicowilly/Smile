#if !defined(HOMINID_TEST_HPP__)
#define HOMINID_TEST_HPP__

#include <string>

namespace hominid
{

class TestResult;

class Test
{
public:
    Test(const std::string& name);
    Test(const Test& test);
    virtual ~Test();

    Test& operator= (const Test& test);

    virtual Test* createDuplicate() const = 0;
    virtual int getTestCount() const = 0;
    const std::string& getName() const;
    virtual bool hasNext() const = 0;
    virtual Test& next() = 0;
    virtual void reset() = 0;
    virtual void run(TestResult& result) = 0;

private:
    std::string m_name;
};

inline const std::string& Test::getName() const
{
    return m_name;
}

}

#endif
