#include "repetition.hpp"
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::repetition::findmatch(scanner& s)
{
    std::vector<std::shared_ptr<match::match>> matches;
    std::shared_ptr<match::match> current;

    int column = s.charno();

    while (current = pat->find(s)) {
        matches.push_back(current);
    }

    return std::shared_ptr<match::match>(new match::sequence(column, type, matches));
}
