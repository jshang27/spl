#ifndef SPL_ERROR_2023
#define SPL_ERROR_2023

#include <string>

#include "token.hpp"

namespace spl
{
    void error(std::string message);
    void error(token tok, std::string message);

    const int FAIL = -1L;
    const int SUCCESS = 0L;
}

#endif
