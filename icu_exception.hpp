#ifndef SMILE_ICU_EXCEPTION_HPP
#define SMILE_ICU_EXCEPTION_HPP

#include <unicode/utypes.h>

#include <stdexcept>

namespace smile
{

class icu_exception : public std::exception
{
public:
    icu_exception(const std::string& msg, UErrorCode uerr);

    virtual const char* what() const noexcept override;

private:
    std::string what_;
};

}

#endif
