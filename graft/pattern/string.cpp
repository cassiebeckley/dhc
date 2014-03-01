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

icu::UnicodeString dhc::graft::pattern::string::str() const
{
    return str_pat;
}

void dhc::graft::pattern::string::initialize()
{
    UChar32 buffer[500];
    UErrorCode e;

    str_pat.toUTF32(buffer, sizeof(buffer), e);

    for (int i = 0; i < sizeof(buffer); i++) {
        std::shared_ptr<character> pat (new character(buffer[i], -1));
        this->pat.push_back(pat);
    }
}
