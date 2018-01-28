#include "demangle.hpp"

#if defined(__clang__) || defined(__GNUC__)
#include <cxxabi.h>
#include <cstdlib>
#elif defined(__SUNPRO_CC)
#include <demangle.h>
#endif

namespace smile
{

namespace demangle
{

std::string demangled_name(const std::type_info& info)
{
#if defined(__clang__) || defined(__GNUC__)
    // On FreeBSD, the __cxa_demangle function sometimes will simply
    // not touch status on success, so we make sure to set it to
    // zero first.
    int status = 0;
    char* demangled = ::abi::__cxa_demangle(info.name(), nullptr, nullptr, &status);
    if (status != 0)
        return info.name();
    std::string result(demangled);
    std::free(demangled);
    return result;
#elif defined(__SUNPRO_CC)
    char buf[1024];
    std::string lnm = std::string("_Z") + info.name();
    return (cplus_demangle(lnm.c_str(), buf, sizeof(buf)) == 0) ?
        std::string(buf) : info.name();
#else
    return info.name();
#endif
}

}

}

