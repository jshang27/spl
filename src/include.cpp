#include <optional>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "include.hpp"

std::optional<std::string> spl::include(const char *filename)
{
    if (!spl::file_exists(filename))
    {
        spl::error("input file does not exist\n");
        return {};
    }

    std::string contents;
    {
        std::ifstream ifs(filename);
        if (ifs.fail())
        {
            spl::error("input file inaccessible or does not exist\n");
            return {};
        }
        std::stringstream ss;
        ss << ifs.rdbuf();
        contents = ss.str();
    }
    return contents;
}