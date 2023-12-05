#include "token.hpp"

spl::token::token(toktype type, int value, std::string name)
{
    this->_type = type;
    this->_value = value;
    this->_name = name;
    this->_next = spl::none_token;
    this->_jmpto = spl::none_token;
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