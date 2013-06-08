#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::character::find(scanner& s)
{
    try {
        char current = s.peek();

        if (current == char_match->data) {
            s.get();
            return std::static_pointer_cast<match::match>(char_match);
        }
    } catch (std::out_of_range& e) {
        return nullptr;
    }

    return nullptr;
}

std::string dhc::lexer::pattern::character::str() const
{
    std::stringstream ss;
    ss << char_match->data;
    return ss.str();
}
