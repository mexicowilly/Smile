#include "smile/AS400.hpp"
#include "AS400Impl.hpp"

namespace smile
{

AS400::AS400(const Text& systemName, const Text& userName, const Text& password)
    : m_impl(new AS400Impl(*this, systemName, userName, password))
{
}

AS400::~AS400()
{
    delete m_impl;
}

int AS400::getCCSID()
{
    return m_impl->getCCSID();
}

const Text& AS400::getSystemName() const
{
    return m_impl->getSystemName();
}

const Text& AS400::getUserName() const
{
    return m_impl->getUserName();
}

const OS400Version& AS400::getVersion()
{
    return m_impl->getVersion();
}

}
