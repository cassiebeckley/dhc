#include "lit_string.hpp"

#include <sstream>

unsigned int dhc::lexer::lit_string::length()
{
    return data.size() + 2; // + 2 because of the two quotes
}

icu::UnicodeString dhc::lexer::lit_string::flatten()
{
    icu::UnicodeString res;
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        res.append((*it)->data);
    }
    return res;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::lit_string::children()
{
    std::vector<std::shared_ptr<dhc::graft::match::match>> res;

    for (auto it = data.begin(); it != data.end(); ++it)
    {
        res.push_back(*it);
    }

    return res;
}
