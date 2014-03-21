#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::character::findmatch(scanner::scanner& s)
{
    int state = s.index;

    try
    {
        std::shared_ptr<match::character> current = std::dynamic_pointer_cast<match::character>(s.next());

        if (current && current->data == pat)
            return current;
    }
    catch (std::out_of_range& e)
    {
        // ignore and return nullptr
    }

    s.index = state;

    return nullptr;
}
