#include "type.hpp"

#include <stdexcept>
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::type::findmatch(scanner::scanner& s)
{
    auto state = s.index;

    try
    {
        std::shared_ptr<graft::match::match> current = s.next();

        if (current && (pat != -1 ? current->type == pat : true) && (str != "" ? current->flatten() == str : true))
            return current;
    }
    catch (std::out_of_range& e)
    {
        // ignore and return nullptr
    }

    s.index = state;

    return nullptr;
}
