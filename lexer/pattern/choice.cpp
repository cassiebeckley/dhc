#include "choice.hpp"

#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::choice::find(scanner& s)
{
    std::shared_ptr<match::match> longest;

    int old_index = s.get_index();
    int max_length = 0;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        std::shared_ptr<match::match> match = (*it)->find(s);

        if (match && (match->length() > max_length)) {
            longest = match;
            max_length = s.get_index() - old_index;
        }
        s.set_index(old_index);
    }

    s.set_index(old_index + max_length);

    return longest;
}

std::string dhc::lexer::pattern::choice::str() const
{
    std::stringstream ss;
    ss << "(";

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        if (it != pat.begin())
            ss << " | ";
        ss << (*it)->str();
    }
    ss << ")";

    return ss.str();
}
