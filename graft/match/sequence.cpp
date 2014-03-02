#include "sequence.hpp"

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
