#ifndef SPL_INCLUDE_2023
#define SPL_INCLUDE_2023

#include <string>
#include <optional>
#include <sys/stat.h>

#include "error.hpp"

namespace spl
{
    std::optional<std::string> include(const char *filename);

    inline bool file_exists(const std::string &name)
    {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }
}

#endif