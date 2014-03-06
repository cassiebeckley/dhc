#include "string.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::string::find(scanner& s)
{
    std::shared_ptr<match::match> comp (compound::find(s));
    if (comp) {
        return std::shared_ptr<match::match> (new match::string(comp->column, type, str_pat));
    } else {
        return nullptr;
    }
}

std::string dhc::graft::pattern::string::str() const
{
    std::string s;
    str_pat.toUTF8String(s);
    return s;
}
