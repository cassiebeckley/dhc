#include "choice.hpp"

#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::choice::findmatch(scanner::scanner& s)
{
    std::shared_ptr<match::match> longest;

    scanner::scanstate old_state = s.state();
    scanner::scanstate max_state = old_state;

    int max_length = 0;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        std::shared_ptr<match::match> match = (*it)->find(s);

        if (match)
        {
            int current_length = match->length();
            if (current_length > max_length)
            {
                longest = match;
                max_length = current_length;
                max_state = s.state();
            }
        }
        s.state() = old_state;
    }

    s.state() = max_state;

    return longest;
}
