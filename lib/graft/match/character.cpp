#include "character.hpp"

#include <sstream>

unsigned int dhc::graft::match::character::length()
{
    return 1;
}

icu::UnicodeString dhc::graft::match::character::flatten()
{
    icu::UnicodeString result;

    if (data != '\r')
        result.append(data);

    return result;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::graft::match::character::children()
{
    return {};
}
