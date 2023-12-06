#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <iostream>

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
    {"write", spl::WRITE},
    {"dup", spl::DUP},
    {"pop", spl::POP},
    {"get", spl::GET},
    {"peek", spl::PEEK},
    {"size", spl::SIZE},
    {"dump", spl::DUMP},
    {"+", spl::ADD},
    {"-", spl::SUB},
    {"*", spl::MUL},
    {"/", spl::DIV},
    {"%", spl::MOD},
    {"==", spl::EQ},
    {"<", spl::LT},
    {"<=", spl::LE},
    {">=", spl::GE},
    {">", spl::GT},
    {"!=", spl::NE},
    {"&", spl::AND},
    {"^", spl::XOR},
    {"|", spl::OR},
    {"!", spl::NOT},
    {"<<", spl::RSHIFT},
    {">>", spl::LSHIFT}};

std::unordered_map<char, char> escape_chars = {
    {'n', '\n'},
    {'r', '\r'},
    {'b', '\b'},
    {'t', '\t'},
    {'e', '\033'},
    {'"', '"'},
    {'\\', '\\'}};

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
                spl::error(spl::token(spl::UNDEF, filename, column, row), "numeric value out of range");
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
    contents += " "; // trust

    size_t contents_size = contents.size();
    std::shared_ptr<spl::token> first = std::make_shared<spl::token>(spl::UNDEF, filename, 0, 0);
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
            ss.str(std::string());

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
                    unassigned_endables.pop_back();
                    break;
                }
                case spl::WHILE:
                {
                    current->next()->set_type(spl::WEND);
                    unassigned_endables.pop_back();
                    break;
                }
                default: // should really only be IF
                {
                    unassigned_endables.pop_back();
                    break;
                }
                }
            }
            current = current->next();
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
                spl::error(spl::token(spl::UNDEF, filename, column, row), "include space betwen strings and other tokens\n");
                return spl::none_token;
            }
            if (contents_size - 2 == i)
            {
                spl::error(spl::token(spl::UNDEF, filename, column, row), "unexpected quote\n");
                return spl::none_token;
            }
            char c = contents[++i];
            column++;
            std::stringstream cur_str_stream;
            while (c != '"')
            {
                if (c == '\\')
                {
                    if (contents_size - 2 == i)
                    {
                        spl::error(spl::token(spl::UNDEF, filename, column, row), "unexpected backslash\n");
                        return spl::none_token;
                    }
                    if (contents[++i] == '0')
                    {
                        c = '\0';
                    }
                    else
                    {
                        c = escape_chars[contents[i]];
                        if (c == 0)
                        {
                            spl::error(spl::token(spl::UNDEF, filename, column, row), "unknown escape char");
                        }
                    }
                }
                else if (c == '\n')
                {
                    spl::error(spl::token(spl::UNDEF, filename, column, row), "unterminated string\n");
                }
                cur_str_stream << c;
                if (contents_size - 2 == 0)
                {
                    spl::error(spl::token(spl::UNDEF, filename, column, row), "unterminated string\n");
                    return spl::none_token;
                }
                c = contents[++i];
                column++;
            }
            std::string cur_str = cur_str_stream.str();
            current->set_next(std::make_shared<spl::token>(spl::PUSH_STR, filename, column, row, cur_str.size(), cur_str));
            current = current->next();
        }
        else
        {
            ss << contents[i];
        }
    }
    if (!unassigned_endables.empty())
    {
        spl::error(*current, "expected `end`\n");
        return spl::none_token;
    }
    return first->next();
}