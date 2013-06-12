#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::character::find(scanner& s)
{
    try {
        std::shared_ptr<match::character> current = std::dynamic_pointer_cast<match::character>(s.lookahead());

        if (!current)
            return nullptr;

        if (current->data == pat) {
            s.consume();
            return current;
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
