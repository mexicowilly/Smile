#if !defined(SMILE_PROGRAMPARAMETERLIST_HPP__)
#define SMILE_PROGRAMPARAMETERLIST_HPP__

#include <smile/ProgramParameter.hpp>
#include <vector>

namespace smile
{

class ProgramParameterList
{
public:
    ProgramParameterList();
    ProgramParameterList(const ProgramParameterList& parameters);
    ~ProgramParameterList();

    ProgramParameterList& operator= (const ProgramParameterList& parameters);
    ProgramParameter& operator[] (unsigned index);
    const ProgramParameter& operator[] (unsigned index) const;

    void addParameter(const ProgramParameter& parameter);
    void addParameterList(const ProgramParameterList& parameters);
    size_t getSize() const;
    void removeParameter(unsigned index);

private:
    std::vector<ProgramParameter*> m_parameters;
};

inline size_t ProgramParameterList::getSize() const
{
    return m_parameters.size();
}

}

#endif
