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

void dhc::graft::pattern::string::initialize()
{
    UChar32 buffer[500];
    UErrorCode e = U_ZERO_ERROR;

    str_pat.toUTF32(buffer, sizeof(buffer), e);
    if (U_FAILURE(e))
    {
        std::cerr << "string::initialize error: " << u_errorName(e) << std::endl;
        return;
    }

    for (unsigned int i = 0; i < sizeof(buffer); i++) {
        std::shared_ptr<character> pat (new character(buffer[i], -1));
        this->pat.push_back(pat);
    }
}
