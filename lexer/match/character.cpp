#include "character.hpp"

#include <sstream>

unsigned int dhc::lexer::match::character::length()
{
    return 1;
}

std::string dhc::lexer::match::character::flatten()
{
    std::stringstream ss;
    if (data == '\r')
        ss << "";
    else
        ss << data;
    return ss.str();
}
