#include "string.hpp"

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::string::find(scanner& s)
{
    if (compound::find(s)) {
        return str_match;
    } else {
        return nullptr;
    }
}

std::string dhc::lexer::pattern::string::str() const
{
    return str_match->data;
}
