#ifndef SMILE_DEMANGLE_HPP
#define SMILE_DEMANGLE_HPP

#include <typeinfo>
#include <string>

namespace smile
{

namespace demangle
{

std::string demangled_name(const std::type_info& info);

}

}

#endif
