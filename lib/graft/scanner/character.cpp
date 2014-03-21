#include "character.hpp"
#include "../match/character.hpp"

dhc::graft::scanner::character::character(icu::UnicodeString src)
{
    length = src.countChar32();
    source = new UChar32[length];
    UErrorCode e = U_ZERO_ERROR;
    src.toUTF32(source, length, e);

    if (U_FAILURE(e))
    {
        //std::cerr << "scanner::character error: " << u_errorName(e) << std::endl;
    }
}


// TODO: perhaps just use a vector for this?
dhc::graft::scanner::character::~character()
{
    delete [] source;
}

std::shared_ptr<dhc::graft::match::match> dhc::graft::scanner::character::next()
{
    if (!finished())
    {
        UChar32 c = source[index++];
        // std::cout << "U+" << std::setw(4) << std::setfill('0') << std::hex << c << std::endl;
        if (c != 0xfeff)
            return std::make_shared<match::character>(-1, c);
        else
            return next();
    }

    return nullptr;
}

bool dhc::graft::scanner::character::finished()
{
    return index >= length;
}
