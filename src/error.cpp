#include <iostream>
#include <string>

#include "error.hpp"

void spl::error(std::string message)
{
    std::cerr << "\e[31merror: \e[0m" << message;
}