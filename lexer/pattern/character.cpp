#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::character::find(scanner& s)
{
    try {
        char current = s.peek();

        if (current == pat) {
            s.get();
            return std::shared_ptr<match::match> (new match::character(s.charno(), type, pat));
        }
    } catch (std::out_of_range& e) {
        return nullptr;
    }

    return nullptr;
}

std::string dhc::lexer::pattern::character::str() const
{
    std::stringstream ss;
    ss << pat;
    return ss.str();
}
