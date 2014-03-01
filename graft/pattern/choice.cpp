#include "choice.hpp"

#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::choice::find(scanner& s)
{
    std::shared_ptr<match::match> longest;

    auto old_state = s.get_state();
    auto max_length = old_state;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        std::shared_ptr<match::match> match = (*it)->find(s);

        if (match && (match->length() > max_length.index - old_state.index)) {
            longest = match;
            max_length = s.get_state();
        }
        s.set_state(old_state);
    }

    s.set_state(max_length);

    if (type != -1 && longest)
        longest->type = type;

    return longest;
}

icu::UnicodeString dhc::graft::pattern::choice::str() const
{
    icu::UnicodeString result;
    result.append('(');

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        if (it != pat.begin())
            result.append(" | ");
        result.append((*it)->str());
    }
    result.append(')');

    return result;
}
