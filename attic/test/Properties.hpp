#if !defined(SMILE_TEST_PROPERTIES_HPP__)
#define SMILE_TEST_PROPERTIES_HPP__

#include <map>
#include <string>
#include <exception>

class Properties
{
public:
    Properties(const std::string& fileName);

    std::string get(const std::string& key);

private:
    std::map<std::string, std::string> m_map;
};

inline std::string Properties::get(const std::string& key)
{
    return m_map[key];
}

#endif
