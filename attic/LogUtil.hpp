#if !defined(SMILE_LOGHELPER_HPP__)
#define SMILE_LOGHELPER_HPP__

#include <smile/Logger.hpp>
#include <sstream>
#include <vector>
#include <stdint.h>

namespace smile
{

namespace LogUtil
{

Logger& smileLogger();
std::string bytes(const std::vector<uint8_t>& bytes);

}

}

#endif
