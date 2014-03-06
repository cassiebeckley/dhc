#include "string.hpp"

unsigned int dhc::graft::match::string::length()
{
    return data.countChar32();
}

icu::UnicodeString dhc::graft::match::string::flatten()
{
    return data;
}
