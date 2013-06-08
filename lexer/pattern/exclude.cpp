#include "exclude.hpp"
#include <sstream>

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::exclude::find(scanner& s)
{
    int old_index = s.get_index();
    std::shared_ptr<match::match> ex_match (exc->find(s));
    s.set_index(old_index);

    std::shared_ptr<match::match> pat_match (pat->find(s));

    if (!pat_match) {
        return nullptr;
    } if (ex_match && ex_match->length() >= pat_match->length()) {
        s.set_index(old_index);
        return nullptr;
    } else {
        return pat_match;
    }
}

std::string dhc::lexer::pattern::exclude::str() const
{
    std::stringstream ss;

    ss << *pat << "^(" << *exc << ")";

    return ss.str();
}
