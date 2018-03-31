#include <hominid/TestSuite.hpp>
#include <hominid/TestCase.hpp>

namespace hominid
{

TestSuite::TestSuite(const std::string& name)
    : Test(name), m_current(m_tests.end()), m_firstTopLevel(false)
{
}

TestSuite::TestSuite(const TestSuite& suite)
    : Test(suite)
{
    for (std::list<Test*>::const_iterator i = suite.m_tests.begin();
         i != suite.m_tests.end();
         i++)
    {
        m_tests.push_back((*i)->createDuplicate());
    }
    m_current = m_tests.begin();
    setFirstTopLevel();
}

TestSuite::~TestSuite()
{
    clear();
}

TestSuite& TestSuite::operator= (const TestSuite& suite)
{
    if (this != &suite)
    {
        Test::operator=(suite);
        clear();
        for (std::list<Test*>::const_iterator i = suite.m_tests.begin();
             i != suite.m_tests.end();
             i++)
        {
            m_tests.push_back((*i)->createDuplicate());
        }
        m_current = m_tests.begin();
        setFirstTopLevel();
    }
    return *this;
}

void TestSuite::addTest(const Test& test)
{
    m_tests.push_back(test.createDuplicate());
    reset();
}

void TestSuite::addTestSuite(const TestSuite& suite)
{
    for (std::list<Test*>::const_iterator i = suite.m_tests.begin();
         i != suite.m_tests.end();
         i++)
    {
        m_tests.push_back((*i)->createDuplicate());
    }
    reset();
}

void TestSuite::clear()
{
    for (std::list<Test*>::iterator itor = m_tests.begin();
         itor != m_tests.end();
         itor++)
    {
        delete *itor;
    }
    m_tests.clear();
    reset();
}

Test* TestSuite::createDuplicate() const
{
    return new TestSuite(*this);
}

void TestSuite::filter(const std::vector<std::string>& names)
{
    TestSuite filtered(getName());
    bool inDesiredClass = false;
    for (std::vector<std::string>::const_iterator itor = names.begin();
         itor != names.end();
         itor++)
    {
        std::string cls;
        std::string test;
        size_t sep = (*itor).find("::");
        if (sep != std::string::npos)
        {
            cls = (*itor).substr(0, sep);
            test = (*itor).substr(sep + 2);
        }
        else
        {
            test = *itor;
        }
        reset();
        while (hasNext())
        {
            Test& cur = next();
            if (cls.empty())
            {
                if (cur.getName() == test)
                    filtered.addTest(cur);
            }
            else
            {
                if (dynamic_cast<TestSuite*>(&cur) != 0)
                {
                    inDesiredClass = (cur.getName() == cls) ? true : false;
                }
                else if (dynamic_cast<TestCase*>(&cur) != 0)
                {
                    if (inDesiredClass && cur.getName() == test)
                        filtered.addTest(cur);
                }
            }
        }
    }
    *this = filtered;
    reset();
}

int TestSuite::getTestCount() const
{
    int count = 0;
    for (std::list<Test*>::const_iterator i = m_tests.begin();
         i != m_tests.end();
         i++)
    {
        count += (*i)->getTestCount();
    }
    return count;
}

bool TestSuite::hasNext() const
{
    return m_current != m_tests.end() &&
           (m_firstTopLevel || (*m_current)->hasNext());
}

Test& TestSuite::next()
{
    Test* test;
    if (m_firstTopLevel)
    {
        test = *m_current;
        m_firstTopLevel = false;
    }
    else
    {
        test = &(*m_current)->next();
    }
    if (!(*m_current)->hasNext())
    {
        m_current++;
        setFirstTopLevel();
    }
    return *test;
}

void TestSuite::reset()
{
    for (std::list<Test*>::iterator itor = m_tests.begin();
         itor != m_tests.end();
         itor++)
    {
        (*itor)->reset();
    }
    m_current = m_tests.begin();
    setFirstTopLevel();
}

void TestSuite::run(TestResult& result)
{
    for (std::list<Test*>::iterator i = m_tests.begin();
         i != m_tests.end();
         i++)
    {
        if (result.shouldStop())
            break;
        (*i)->run(result);
    }
}

void TestSuite::setFirstTopLevel()
{
    m_firstTopLevel = m_current != m_tests.end() &&
        dynamic_cast<TestSuite*>(*m_current) != 0;
}

}
