#if !defined(SMILE_TYPEINFO_HPP__)
#define SMILE_TYPEINFO_HPP__

#include <typeinfo>
#include <string>

namespace smile
{

namespace TypeInfo
{

std::string getName(const std::type_info& ti);

}

}

#endif
