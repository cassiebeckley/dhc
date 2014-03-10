#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::character::findmatch(scanner& s)
{
    auto state = s.state;

    try
    {
        std::shared_ptr<match::character> current = s.next();

        if (current && current->data == pat)
            return current;
    }
    catch (std::out_of_range& e)
    {
        // ignore and return nullptr
    }

    s.state = state;

    return nullptr;
}
