#include "smile/ProgramParameterList.hpp"
#include <assert.h>

namespace smile
{

ProgramParameterList::ProgramParameterList()
{
}

ProgramParameterList::ProgramParameterList(const ProgramParameterList& parameters)
{
    for (std::vector<ProgramParameter*>::const_iterator itor = parameters.m_parameters.begin();
         itor != parameters.m_parameters.end();
         itor++)
    {
        m_parameters.push_back((*itor)->duplicate());
    }
}

ProgramParameterList::~ProgramParameterList()
{
    for (std::vector<ProgramParameter*>::iterator itor = m_parameters.begin();
         itor != m_parameters.end();
         itor++)
    {
        delete *itor;
    }
}

ProgramParameterList& ProgramParameterList::operator= (const ProgramParameterList& parameters)
{
    for (std::vector<ProgramParameter*>::iterator itor = m_parameters.begin();
         itor != m_parameters.end();
         itor++)
    {
        delete *itor;
    }
    m_parameters.clear();
    for (std::vector<ProgramParameter*>::const_iterator itor = parameters.m_parameters.begin();
         itor != parameters.m_parameters.end();
         itor++)
    {
        m_parameters.push_back((*itor)->duplicate());
    }
    return *this;
}

ProgramParameter& ProgramParameterList::operator[] (unsigned index)
{
    assert(index < m_parameters.size());
    return *m_parameters[index];
}

const ProgramParameter& ProgramParameterList::operator[] (unsigned index) const
{
    assert(index < m_parameters.size());
    return *m_parameters[index];
}

void ProgramParameterList::addParameter(const ProgramParameter& parameter)
{
    m_parameters.push_back(parameter.duplicate());
}

void ProgramParameterList::addParameterList(const ProgramParameterList& parameters)
{
    for (unsigned i = 0; i < parameters.getSize(); i++)
        m_parameters.push_back(parameters[i].duplicate());
}

void ProgramParameterList::removeParameter(unsigned index)
{
    assert(index < m_parameters.size());
    delete m_parameters[index];
    m_parameters.erase(m_parameters.begin() + index);
}

}
