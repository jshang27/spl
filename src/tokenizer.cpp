#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "token.hpp"
#include "tokenizer.hpp"
#include "error.hpp"

std::unordered_map<std::string, spl::toktype> keywords = {
    {"function", spl::FDEF},
    {"return", spl::RET},
    {"any", spl::ANY},
    {"->", spl::ARROW},
    {"end", spl::END},
    {"while", spl::WHILE},
    {"if", spl::IF},
    {"else", spl::ELSE},
    {"write", spl::WRITE}};

bool is_digit(std::string word)
{
    size_t word_size = word.size();
    for (size_t i = 0; i < word_size; i++)
    {
        if (word[i] < '0' || word[i] > '9')
        {
            return false;
        }
    }
    return true;
}

std::shared_ptr<spl::token> make_token(std::string name, std::string filename, size_t column, size_t row)
{
    spl::toktype type = keywords[name];
    int value = 0;
    if (type == 0)
    {
        if (is_digit(name))
        {
            type = spl::PUSH;
            value = std::stoi(name);
        }
        else
        {
            type = spl::NAME;
        }
    }
    return std::make_shared<spl::token>(type, filename, column, row, value, name);
}

std::shared_ptr<spl::token> spl::tokenize(std::string contents, std::string filename)
{
    spl::error("implemented\n");
    exit(spl::FAIL);

    size_t contents_size = contents.size();
    std::shared_ptr<spl::token> first = std::make_shared<spl::token>(spl::PUSH, filename, 0, 0);
    std::vector<std::shared_ptr<spl::token>> unassigned_endables;
    size_t column = 1;
    size_t row = 1;

    for (size_t i = 0; i < contents_size; i++)
    {
    }
    return first;
}