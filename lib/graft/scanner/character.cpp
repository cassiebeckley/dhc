#include "character.hpp"
#include "../match/character.hpp"

// TODO: remove
#include <iostream>

dhc::graft::scanner::character::character(icu::UnicodeString src)
{
    length = src.countChar32();
    source = new UChar32[length];
    UErrorCode e = U_ZERO_ERROR;
    src.toUTF32(source, length, e);

    if (U_FAILURE(e))
    {
        std::cerr << "scanner::character error: " << u_errorName(e) << std::endl;
    }
}

dhc::graft::scanner::character::~character()
{
    delete [] source;
}

std::shared_ptr<dhc::graft::match::match> dhc::graft::scanner::character::next()
{
    if (!finished())
    {
        return std::make_shared<match::character>(state_column()++, -1, source[state_index()++]);
    }

    return nullptr;
}

bool dhc::graft::scanner::character::finished()
{
    return state_index() >= length;
}
