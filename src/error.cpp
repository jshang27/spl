#include <iostream>
#include <string>

#include "error.hpp"
#include "token.hpp"

void spl::error(std::string message)
{
    std::cerr << "\033[31merror: \033[0m" << message;
}

void spl::error(spl::token tok, std::string message)
{
    std::cerr << "\033[31merror: \033[0m" << tok.filename() << ":" << tok.row() << ":" << tok.column() << ": " << message;
}