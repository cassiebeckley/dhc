#include "string.hpp"

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::pattern::string::find(scanner& s)
{
    std::shared_ptr<match::match> comp (compound::find(s));
    if (comp) {
        return std::shared_ptr<match::match> (new match::string(comp->column, type, str_pat));
    } else {
        return nullptr;
    }
}

std::string dhc::lexer::pattern::string::str() const
{
    return str_pat;
}

void dhc::lexer::pattern::string::initialize()
{
    for (auto it = str_pat.begin(); it != str_pat.end(); ++it) {
        std::shared_ptr<character> pat (new character(*it, -1));
        this->pat.push_back(pat);
    }
}
