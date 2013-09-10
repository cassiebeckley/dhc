#include "character.hpp"

#include <sstream>

unsigned int dhc::graft::match::character::length()
{
    return 1;
}

std::string dhc::graft::match::character::flatten()
{
    std::stringstream ss;
    if (data == '\r')
        ss << "";
    else
        ss << data;
    return ss.str();
}
