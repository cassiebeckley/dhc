#include "string.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::graft::pattern::string::findmatch(scanner::scanner& s)
{
    std::string flat;
    str_pat.toUTF8String(flat);
    std::shared_ptr<match::match> comp (compound::findmatch(s));
    if (comp) {
        return std::shared_ptr<match::match> (new match::string(type, str_pat));
    } else {
        return nullptr;
    }
}
