#include "lit_char.hpp"

#include <sstream>

unsigned int dhc::lexer::lit_char::length()
{
    return 1;
}

icu::UnicodeString dhc::lexer::lit_char::flatten()
{
    return data;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::lit_char::children()
{
    return {};
}
