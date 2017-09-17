#include "icu_exception.hpp"

namespace smile
{

icu_exception::icu_exception(const std::string& msg, UErrorCode uerr)
    : what_(msg + ": " + u_errorName(uerr))
{
}

const char* icu_exception::what() const noexcept
{
    return what_.c_str();
}

}