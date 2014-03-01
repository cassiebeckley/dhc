#include "string.hpp"

unsigned int dhc::graft::match::string::length()
{
    return data.countChar32();
}

UnicodeString dhc::graft::match::string::flatten()
{
    return data;
}
