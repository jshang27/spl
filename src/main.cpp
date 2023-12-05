#include <optional>
#include <iostream>
#include <memory>

#include "error.hpp"
#include "include.hpp"
#include "token.hpp"
#include "tokenizer.hpp"

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
    if (first == spl::none_token)
    {
        spl::error("compilation failed\n");
        return spl::FAIL;
    }

    while (first != spl::none_token)
    {
        std::cout << first->type() << "\n";
        first = first->next();
    }

    return spl::SUCCESS;
}