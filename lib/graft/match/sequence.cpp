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

icu::UnicodeString dhc::graft::match::sequence::tree(unsigned int indent)
{
    icu::UnicodeString res;
    for (auto it = data.begin(); it != data.end(); ++it)
    {
        for (unsigned int i = 0; i < indent * 4; i++)
            res.append(' ');

        res.append("* ");
        res.append((*it)->tree(indent + 1));
        res.append("\n");
    }

    return res;
}

icu::UnicodeString dhc::graft::match::sequence::flatten()
{
    icu::UnicodeString res;
    for (auto it = data.begin(); it != data.end(); ++it) {
        res.append((*it)->flatten());
    }

    return res;
}
