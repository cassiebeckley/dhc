#include "compound.hpp"
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::compound::find(scanner& s)
{
    auto old_state = s.get_state();
    int column = s.charno();

    std::vector<std::shared_ptr<match::match>> matches;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        std::shared_ptr<match::match> current = (*it)->find(s);

        if (!current) {
            s.set_state(old_state);
            return nullptr;
        } else {
            matches.push_back(current);
        }
    }

    return std::shared_ptr<match::match>(new match::sequence(column, type, matches));
}

icu::UnicodeString dhc::graft::pattern::compound::str() const
{
    icu::UnicodeString result;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        if (it != pat.begin())
            result.append(' ');
        result.append((*it)->str());
    }

    return result;
}

void dhc::graft::pattern::compound::add_pattern(std::shared_ptr<pattern> patt)
{
    pat.push_back(patt);
}
