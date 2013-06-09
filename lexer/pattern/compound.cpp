#include "compound.hpp"
#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::compound::find(scanner& s)
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

std::string dhc::lexer::pattern::compound::str() const
{
    std::stringstream ss;

    for (auto it = pat.begin(); it != pat.end(); ++it) {
        if (it != pat.begin())
            ss << " ";
        ss << (*it)->str();
    }

    return ss.str();
}

void dhc::lexer::pattern::compound::add_pattern(std::shared_ptr<pattern> patt)
{
    pat.push_back(patt);
}
