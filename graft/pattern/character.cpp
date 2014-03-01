#include "character.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::character::find(scanner& s)
{
    auto state = s.get_state();

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

    s.set_state(state);

    return nullptr;
}

icu::UnicodeString dhc::graft::pattern::character::str() const
{
    icu::UnicodeString result;
    result.append(pat);
    return result;
}
