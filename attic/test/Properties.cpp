#include "Properties.hpp"
#include <fstream>
#include <stdexcept>
#include <regex.h>

Properties::Properties(const std::string& fileName)
{
    std::ifstream stream(fileName.c_str());
    if (!stream.good())
        throw std::runtime_error("Could not open test properties file");
    regex_t commentPattern;
    if (regcomp(&commentPattern, "^[[:space:]]*#", REG_EXTENDED | REG_NOSUB) != 0)
        throw std::runtime_error("Could not compile comment pattern");
    regex_t contentPattern;
    if (regcomp(&contentPattern,
                "^[[:space:]]*([[:graph:]]+)[[:space:]]*=[[:space:]]*([[:graph:]]+.*)[[:space:]]*$",
                REG_EXTENDED) != 0)
    {
        regfree(&commentPattern);
        throw std::runtime_error("Could not compile content pattern");
    }
    char buf[8192];
    regmatch_t matches[3];
    while (true)
    {
        stream.getline(buf, 8192);
        if (!stream.good())
            break;
        if (regexec(&commentPattern, buf, 0, 0, 0) == 0)
            continue;
        if (regexec(&contentPattern, buf, 3, matches, 0) == 0)
        {
            size_t keySize = matches[1].rm_eo - matches[1].rm_so;
            size_t valueSize = matches[2].rm_eo - matches[2].rm_so;
            if (keySize > 0)
            {
                m_map[std::string(buf + matches[1].rm_so, keySize)] =
                    std::string(buf + matches[2].rm_so, valueSize);
            }
        }
    }
    regfree(&commentPattern);
    regfree(&contentPattern);
}
