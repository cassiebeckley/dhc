#include "digit.hpp"

#include <sstream>

unsigned int dhc::lexer::digit::length()
{
    return 1;
}

icu::UnicodeString dhc::lexer::digit::flatten()
{
    icu::UnicodeString result;

    result.append(data);

    return result;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::digit::children()
{
    return {};
}
