#include <optional>
#include <iostream>

#include "error.hpp"
#include "include.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        spl::error("improper usage: expected a file\n");
        return spl::FAIL;
    }

    std::optional<std::string> contents = spl::include(argv[1]);
    if (!contents.has_value())
    {
        spl::error("compilation failed\n");
        return spl::FAIL;
    }
    std::cout << contents.value() << std::endl;

    return spl::SUCCESS;
}