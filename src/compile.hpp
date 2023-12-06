#ifndef SPL_COMPILE_2023
#define SPL_COMPILE_2023

#include <fstream>

#include "token.hpp"

namespace spl
{
    int compile_to_asm(std::string ofn, std::shared_ptr<spl::token> first);
}

#endif