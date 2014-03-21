#include "exclude.hpp"
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::exclude::findmatch(scanner::scanner& s)
{
    int old_state = s.index;
    std::shared_ptr<match::match> ex_match (exc->find(s));
    s.index = old_state;

    std::shared_ptr<match::match> pat_match (pat->find(s));

    if (!pat_match) {
        return nullptr;
    } if (ex_match && ex_match->length() >= pat_match->length()) {
        s.index = old_state;
        return nullptr;
    } else {
        return pat_match;
    }
}
