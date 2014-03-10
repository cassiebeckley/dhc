#include "pattern.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::pattern::find(scanner::scanner &s)
{
    auto match = findmatch(s);

    if (match && process)
    {
        return process(match);
    }
    else
    {
        return match;
    }
}
