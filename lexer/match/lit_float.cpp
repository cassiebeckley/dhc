#include "lit_float.hpp"

#include <sstream>

unsigned int dhc::lexer::lit_float::length()
{
    return len;
}

icu::UnicodeString dhc::lexer::lit_float::flatten()
{
    std::stringstream ss;
    ss << integral << '.' << fractional << 'e' << exponent;
    std::string str = ss.str();

    icu::UnicodeString result;

    for (auto it = str.begin(); it != str.end(); ++it)
    {
        result.append(*it);
    }

    return result;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::lit_float::children()
{
    return {};
}
