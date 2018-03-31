#if !defined(SMILE_OS400NAMEVALUEPAIRRETRIEVER_HPP__)
#define SMILE_OS400NAMEVALUEPAIRRETRIEVER_HPP__

#include <smile/AS400.hpp>
#include <smile/OS400Value.hpp>
#include <map>

namespace smile
{

class OS400ValueManipulator
{
public:
    static OS400ValueManipulator& getInstance();

    std::map<Text, OS400GenericValue*> retrieveValues(AS400& as400,
                                                      const std::vector<Text> names,
                                                      bool systemValues);
    void setValue(AS400& as400,
                  const Text& name,
                  const OS400GenericValue& value,
                  bool systemValue);

protected:
    friend class StaticInitializer;

    OS400ValueManipulator();

private:
    enum ValueType
    {
        ValueType_Text,
        ValueType_TextArray,
        ValueType_Int32
    };

    class ValueMetadata
    {
    public:
        ValueMetadata();
        ValueMetadata(ValueType type, size_t unitSize);
        ValueMetadata(ValueType type, size_t unitSize, size_t arraySize);
        ValueMetadata(const ValueMetadata& metadata);

        ValueMetadata& operator= (const ValueMetadata& metadata);

        size_t getApiSize() const;
        size_t getArraySize() const;
        size_t getTotalSize() const;
        ValueType getType() const;
        size_t getUnitSize() const;

    private:
        ValueType m_type;
        size_t m_unitSize;
        size_t m_arraySize;
    };

    OS400ValueManipulator(const OS400ValueManipulator&);
    OS400ValueManipulator& operator= (const OS400ValueManipulator&);

    std::map<Text, OS400GenericValue*> parseOsData(std::vector<uint8_t> data, int ccsid);

    std::map<Text, ValueMetadata> m_metadata;
};

inline size_t OS400ValueManipulator::ValueMetadata::getApiSize() const
{
    return getTotalSize() + 24;
}

inline size_t OS400ValueManipulator::ValueMetadata::getArraySize() const
{
    return m_arraySize;
}

inline size_t OS400ValueManipulator::ValueMetadata::getTotalSize() const
{
    return m_unitSize * m_arraySize;
}

inline OS400ValueManipulator::ValueType OS400ValueManipulator::ValueMetadata::getType() const
{
    return m_type;
}

inline size_t OS400ValueManipulator::ValueMetadata::getUnitSize() const
{
    return m_unitSize;
}

}

#endif
