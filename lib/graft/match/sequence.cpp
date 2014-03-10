#include "sequence.hpp"

#include <sstream>

unsigned int dhc::graft::match::sequence::length()
{
    int l = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        l += (*it)->length();
    }

    return l;
}

icu::UnicodeString dhc::graft::match::sequence::flatten()
{
    icu::UnicodeString res;
    for (auto it = data.begin(); it != data.end(); ++it) {
        res.append((*it)->flatten());
    }

    return res;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::graft::match::sequence::children()
{
    return data;
}
