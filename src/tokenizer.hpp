#ifndef SPL_TOKENIZER_2023
#define SPL_TOKENIZER_2023

#include <memory>
#include <string>

#include "token.hpp"

namespace spl
{
    std::shared_ptr<token> tokenize(std::string contents, std::string filename);
}

#endif