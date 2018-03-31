#if !defined(HOMINID_ASSERTIONFAILURE_HPP__)
#define HOMINID_ASSERTIONFAILURE_HPP__

#include <string>

namespace hominid
{

class AssertionFailure
{
public:
    AssertionFailure(const char* const file, const int line);
    AssertionFailure(const char* const file, const int line, const std::string& message);
    AssertionFailure(const AssertionFailure& failure);

    AssertionFailure& operator= (const AssertionFailure& failure);

    const std::string& getFile() const;
    int getLine() const;
    const std::string& getMessage() const;
    std::string toString() const;

private:
    std::string m_file;
    std::string m_message;
    int         m_line;
};

inline const std::string& AssertionFailure::getFile() const
{
    return m_file;
}

inline int AssertionFailure::getLine() const
{
    return m_line;
}

inline const std::string& AssertionFailure::getMessage() const
{
    return m_message;
}

}

#endif
