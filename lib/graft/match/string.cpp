#include "string.hpp"

unsigned int dhc::graft::match::string::length()
{
    return data.countChar32();
}

icu::UnicodeString dhc::graft::match::string::tree(unsigned int indent)
{
    indent += 2;
    return data;
}

icu::UnicodeString dhc::graft::match::string::flatten()
{
    return data;
}
