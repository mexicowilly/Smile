#include "TypeInfo.hpp"
#include "Config.hpp"
#if defined(SMILE_GCC_DEMANGLING)
#include <cxxabi.h>
#include <assert.h>
#endif

namespace smile
{

std::string TypeInfo::getName(const std::type_info& ti)
{
#if defined(SMILE_GCC_DEMANGLING)
    int status;
    char* demangled = SMILE_CXA_NAMESPACE::__cxa_demangle(ti.name(), 0, 0, &status);
    assert(status != -1);
    assert(status != -2);
    assert(status != -3);
    assert(status == 0);
    std::string output(demangled);
    free(demangled);
    return output;
#else
    return std::string(ti.name());
#endif
}

}
