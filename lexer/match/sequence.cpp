#include "sequence.hpp"

#include <sstream>

unsigned int dhc::lexer::match::sequence::length()
{
    int l = 0;
    for (auto it = data.begin(); it != data.end(); ++it) {
        l += (*it)->length();
    }

    return l;
}

std::string dhc::lexer::match::sequence::flatten()
{
    std::stringstream ss;
    for (auto it = data.begin(); it != data.end(); ++it) {
        ss << (*it)->flatten();
    }

    return ss.str();
}
