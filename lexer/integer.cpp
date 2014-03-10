#include "integer.hpp"

#include <sstream>

unsigned int dhc::lexer::integer::length()
{
    return len;
}

icu::UnicodeString dhc::lexer::integer::flatten()
{
    std::stringstream ss;
    ss << data;
    std::string str = ss.str();

    icu::UnicodeString result;

    for (auto it = str.begin(); it != str.end(); ++it)
    {
        result.append(*it);
    }

    return result;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::integer::children()
{
    return {};
}
