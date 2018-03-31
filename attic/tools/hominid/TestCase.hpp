#if !defined(HOMINID_TESTCASE_HPP__)
#define HOMINID_TESTCASE_HPP__

#include <hominid/TestSuite.hpp>
#include <sstream>

#define HERE __FILE__, __LINE__

#define HOMINID_BEGIN_SUB_SUITE(parent, cls) \
    public: \
    typedef void ( cls ::*TestMethod)(); \
    hominid::Test* createDuplicate() const \
    { \
        return new cls (*this); \
    } \
    static hominid::TestSuite getTestSuite() \
    { \
        hominid::TestSuite suite( #cls ); \
        for (std::map<std::string, TestMethod>::iterator i = getTestMethodMap().begin(); \
             i != getTestMethodMap().end(); \
             i++) \
        { \
            suite.addTest( cls (i->first)); \
        } \
        return suite; \
    } \
    cls (const std::string& name) : parent (name) \
    { \
    } \
    private: \
    typedef cls HOMINID_TestCaseType; \
    static std::map<std::string, TestMethod>& getTestMethodMap() \
    { \
        static std::map<std::string, TestMethod> testMethodMap; \
        if (testMethodMap.empty()) \
        {

#define HOMINID_END_SUB_SUITE \
        } \
        return testMethodMap; \
    } \
    protected: \
    void runTest() \
    { \
        std::map<std::string, TestMethod>::iterator iter = getTestMethodMap().find(getName()); \
        if (iter == getTestMethodMap().end()) \
            fail(__FILE__, __LINE__, getName() + ": No such test method"); \
        (this->*(iter->second))(); \
    } \
    public:

#define HOMINID_TEST(mtd) \
        testMethodMap[ #mtd ] = &HOMINID_TestCaseType:: mtd;

#define HOMINID_BEGIN_SUITE(cls) HOMINID_BEGIN_SUB_SUITE(TestCase, cls)
#define HOMINID_END_SUITE HOMINID_END_SUB_SUITE

#define HOMINID_PREDICATE_NO_THROW(file, line, expr) \
    try \
    { \
        expr ; \
    } \
    catch (std::exception& e) \
    { \
        fail( file , line , std::string("Unexpected exception caught: ") + e.what()); \
    } \
    catch (...) \
    { \
        fail( file , line , "Completely unexpected exception caught"); \
    }


#define HOMINID_PREDICATE_THROW(file, line, expr, except) \
    try \
    { \
        expr ; \
        fail( file , line , "No exception thrown"); \
    } \
    catch ( except & e) \
    { \
    } \
    catch (std::exception& stde) \
    { \
        fail( file , line , std::string("Unexpected exception caught: ") + stde.what()); \
    } \
    catch (...) \
    { \
        fail( file , line , "Completely unexpected exception caught"); \
    }


namespace hominid
{

class TestCase : public Test
{
public:
    typedef TestSuite (*TestSuiteFactory)();

    TestCase(const std::string& name);

    virtual int getTestCount() const;
    virtual bool hasNext() const;
    virtual Test& next();
    virtual void reset();
    virtual void run(TestResult& result);
    void runBare();
    virtual void setUp();
    virtual void tearDown();

protected:
    void fail(const char* const file,
              int line,
              const std::string& message = std::string());
    void predicate(const char* const file,
                   int line,
                   bool condition,
                   const std::string& message = std::string());
    template <class T> void predicateEquals(const char* const file,
                                            int line,
                                            const T& expected,
                                            const T& got,
                                            const std::string& message = std::string());
    virtual void runTest() = 0;

private:
    bool m_hasNext;
};

template <class T>
void TestCase::predicateEquals(const char* const file,
                               int line,
                               const T& expected,
                               const T& got,
                               const std::string& message)
{
    if (expected != got)
    {
        std::ostringstream strm;
        if (!message.empty())
            strm << message << " ";
        strm << "Expected " << expected << ", but got " << got << ".";
        fail(file, line, strm.str());
    }
}

}

#endif
