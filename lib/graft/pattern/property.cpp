#include "property.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::property::find(scanner& s)
{
    auto state = s.get_state();

    try
    {
        std::shared_ptr<match::character> current = s.next();

        if (current && pat.contains(current->data))
            return current;
    }
    catch (std::out_of_range& e)
    {
        // ignore and return nullptr
    }

    s.set_state(state);

    return nullptr;
}


// TODO: consider removing these
std::string dhc::graft::pattern::property::str() const
{
    std::string s;
    icu::UnicodeString us;
    pat.toPattern(us);
    us.toUTF8String(s);
    return s;
}
