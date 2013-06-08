#include "string.hpp"

int dhc::lexer::match::string::length()
{
    return data.length();
}

std::string dhc::lexer::match::string::flatten()
{
    return data;
}
