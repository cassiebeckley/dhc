#include "pattern.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::pattern::find(scanner::scanner &s)
{
    auto match = findmatch(s);

    if (match)
    {
        if (type != -1)
            match->type = type;

        if (process)
        {
            return process(match);
        }
    }

    return match;
}
