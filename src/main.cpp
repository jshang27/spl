#include <optional>
#include <iostream>
#include <memory>

#include "error.hpp"
#include "include.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "compile.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        spl::error("improper usage: expected a file\n");
        return spl::FAIL;
    }

    std::optional<std::string> contents = spl::get_contents(argv[1]);
    if (!contents.has_value())
    {
        spl::error("compilation failed\n");
        return spl::FAIL;
    }

    std::shared_ptr<spl::token> first = spl::tokenize(contents.value(), argv[1]);
    if (first->type() == spl::UNDEF)
    {
        spl::error("compilation failed\n");
        return spl::FAIL;
    }

    {
        std::string filename(argv[1]);
        int rightmost = filename.rfind('.');
        if (rightmost == -1)
        {
            spl::error("invalid filename, could not compile");
            return spl::FAIL;
        }
        filename = filename.substr(0, rightmost);

        if (spl::compile_to_asm(filename + ".asm", first) == spl::FAIL)
        {
            spl::error("compilation failed\n");
            return spl::FAIL;
        }
    }

    return spl::SUCCESS;
}