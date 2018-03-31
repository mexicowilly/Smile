#if !defined(SMILE_SYSTEM_HPP__)
#define SMILE_SYSTEM_HPP__

#include <smile/Properties.hpp>

namespace smile
{

class System
{
public:
    static System& getInstance();

    Properties& getProperties();

protected:
    friend class StaticInitializer;

    System();

private:
    System(const System&);
    System& operator= (const System&);

    Properties m_properties;
};

inline Properties& System::getProperties()
{
    return m_properties;
}

}

#endif
