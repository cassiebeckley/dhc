#include "string.hpp"

unsigned int dhc::graft::match::string::length()
{
    return data.length();
}

std::string dhc::graft::match::string::flatten()
{
    return data;
}
