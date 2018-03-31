#if !defined(HOMINID_TESTSUITE_HPP__)
#define HOMINID_TESTSUITE_HPP__

#include <hominid/TestResult.hpp>

namespace hominid
{

class TestSuite : public Test
{
public:
    TestSuite(const std::string& name);
    TestSuite(const TestSuite& suite);
    ~TestSuite();

    TestSuite& operator= (const TestSuite& suite);

    void addTest(const Test& test);
    void addTestSuite(const TestSuite& suite);
    void clear();
    virtual Test* createDuplicate() const;
    void filter(const std::vector<std::string>& names);
    virtual int getTestCount() const;
    virtual bool hasNext() const;
    virtual Test& next();
    virtual void reset();
    virtual void run(TestResult& result);

private:
    void setFirstTopLevel();

    std::list<Test*> m_tests;
    std::list<Test*>::const_iterator m_current;
    bool m_firstTopLevel;
};

}

#endif
