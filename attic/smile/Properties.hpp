#if !defined(SMILE_PROPERTIES_HPP__)
#define SMILE_PROPERTIES_HPP__

#include <smile/Text.hpp>
#include <map>
#include <algorithm>

namespace smile
{

class Properties
{
public:
    static Text CHANGE_NOTIFICATION;
    static Text PROPERTY_NAME;
    static Text OLD_VALUE;
    static Text NEW_VALUE;

    Properties();
    Properties(const char* const fileName, const char* const codepage = "UTF-8");
    Properties(const Properties& props);

    Properties& operator= (const Properties& props);
    bool operator== (const Properties& props) const;
    bool operator!= (const Properties& props) const;

    const Text& get(const Text& property) const;
    std::vector<std::pair<Text, Text> > getAll() const;
    bool getBool(const Text& property) const;
    int64_t getInt(const Text& property)const;
    bool hasProperty(const Text& property) const;
    void merge(const Properties& properties);
    void set(const Text& property, const Text& value);
    void setBool(const Text& property, bool value);
    void setInt(const Text& property, int64_t value);
    size_t size() const;

private:
    class MergeValue : public std::unary_function<std::pair<Text, Text>, void>
    {
    public:
        MergeValue(std::map<Text, Text>& properties);

        void operator() (const std::pair<const Text, Text>& p);

    private:
        std::map<Text, Text>& m_properties;
    };
    Text processValue(std::vector<UChar>& value, bool& continuation);

    std::map<Text, Text> m_properties;
    bool m_notify;
};

inline bool Properties::operator!= (const Properties& props) const
{
    return !operator==(props);
}

inline bool Properties::hasProperty(const Text& property) const
{
    return m_properties.find(property) != m_properties.end();
}

inline void Properties::merge(const Properties& properties)
{
    std::for_each(properties.m_properties.begin(),
                  properties.m_properties.end(),
                  MergeValue(m_properties));
}

inline size_t Properties::size() const
{
    return m_properties.size();
}

inline Properties::MergeValue::MergeValue(std::map<Text, Text>& properties)
    : m_properties(properties)
{
}

inline void Properties::MergeValue::operator() (const std::pair<const Text, Text>& p)
{
    m_properties[p.first] = p.second;
}

}

#endif
