#include "character.hpp"

#include <sstream>

unsigned int dhc::lexer::match::character::length()
{
    return 1;
}

std::string dhc::lexer::match::character::flatten()
{
    std::stringstream ss;
    ss << data;
    return ss.str();
}
