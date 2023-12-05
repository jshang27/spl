#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>

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
    if (word[0] != '-' && (word[0] < '0' || word[0] > '9'))
    {
        return false;
    }

    size_t word_size = word.size();
    for (size_t i = 1; i < word_size; i++)
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
    long value = 0;
    if (type == 0)
    {
        if (is_digit(name))
        {
            type = spl::PUSH;
            try
            {
                value = std::stol(name);
            }
            catch (std::out_of_range &)
            {
                spl::error(spl::token(spl::PUSH, filename, column, row), "numeric value out of range");
            }
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
    spl::error("unimplemented\n");
    exit(spl::FAIL);

    size_t contents_size = contents.size();
    std::shared_ptr<spl::token> first = std::make_shared<spl::token>(spl::PUSH, filename, 0, 0);
    std::shared_ptr<spl::token> current = first;
    std::vector<std::shared_ptr<spl::token>> unassigned_endables;
    size_t column = 0;
    size_t row = 1;

    std::stringstream ss;

    for (size_t i = 0; i < contents_size; i++)
    {
        column++;
        if (std::isspace(contents[i]))
        {
            if (ss.str().empty())
            {
                continue;
            }
            current->set_next(make_token(ss.str(), filename, column, row));
            ss.clear();
            toktype ntt = current->next()->type();
            if (ntt == spl::FDEF || ntt == spl::IF || ntt == spl::WHILE)
            {
                unassigned_endables.push_back(current->next());
            }
            else if (ntt == spl::END)
            {
                if (unassigned_endables.empty())
                {
                    spl::error(*current->next(), "unexpected `end`\n");
                    return spl::none_token;
                }
                switch (unassigned_endables.back()->type())
                {
                case spl::FDEF:
                {
                    current->next()->set_type(spl::FEND);
                    break;
                }
                case spl::WHILE:
                {
                    current->next()->set_type(spl::WEND);
                    break;
                }
                default:
                {
                    break;
                }
                }
            }
            if (contents[i] == '\n')
            {
                row++;
                column = 0;
            }
        }
        else if (contents[i] == '"')
        {
            if (!ss.str().empty())
            {
                spl::error(*current, "include space betwen strings and other tokens");
                return spl::none_token;
            }
            // TODO: include string logic
        }
        else
        {
            ss << contents[i];
        }
    }
    return first;
}