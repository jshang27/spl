#include "token.hpp"

spl::token::token(toktype type, std::string filename, size_t column, size_t row, int value, std::string name)
{
    this->_type = type;
    this->_filename = filename;
    this->_column = column;
    this->_row = row;
    this->_value = value;
    this->_name = name;
    this->_next = spl::none_token;
    this->_jmpto = spl::none_token;
}

std::string spl::token::filename()
{
    return this->_filename;
}
void spl::token::set_filename(std::string filename)
{
    this->_filename = filename;
}

size_t spl::token::column()
{
    return this->_column;
}
void spl::token::set_column(size_t column)
{
    this->_column = column;
}

size_t spl::token::row()
{
    return this->_row;
}
void spl::token::set_row(size_t row)
{
    this->_row = row;
}

spl::toktype spl::token::type()
{
    return this->_type;
}

void spl::token::set_type(spl::toktype type)
{
    this->_type = type;
}

int spl::token::value()
{
    return this->_value;
}

void spl::token::set_value(int value)
{
    this->_value = value;
}

std::string spl::token::name()
{
    return this->_name;
}
void spl::token::set_name(std::string name)
{
    this->_name = name;
}

std::shared_ptr<spl::token> spl::token::next()
{
    return this->_next;
}

void spl::token::set_next(std::shared_ptr<spl::token> next)
{
    this->_next = next;
}

std::shared_ptr<spl::token> spl::token::jmp()
{
    return this->_jmpto;
}
void spl::token::set_jmp(std::shared_ptr<spl::token> jmp)
{
    this->_jmpto = jmp;
}