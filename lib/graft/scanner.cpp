#include "scanner.hpp"

// TODO: remove
#include <iostream>

dhc::graft::scanner::scanner::scanner(icu::UnicodeString src)
{
    length = src.countChar32();
    source = new UChar32[length];
    UErrorCode e = U_ZERO_ERROR;
    src.toUTF32(source, length, e);

    if (U_FAILURE(e))
    {
        std::cerr << "scanner::scanner error: " << u_errorName(e) << std::endl;
    }
}

dhc::graft::scanner::scanner::~scanner()
{
    delete [] source;
}

std::shared_ptr<dhc::graft::match::character> dhc::graft::scanner::scanner::next()
{
    if (!finished())
    {
        return std::make_shared<match::character>(state.column++, -1, source[state.index++]);
    }

    return nullptr;
}

bool dhc::graft::scanner::scanner::finished()
{
    return state.index >= length;
}

void dhc::graft::scanner::scanner::newline()
{
    state.line_number++;
    state.column = 0;
}

unsigned int dhc::graft::scanner::scanner::lineno()
{
    return state.line_number;
}

unsigned int dhc::graft::scanner::scanner::charno()
{
    return state.column;
}

unsigned int dhc::graft::scanner::scanner::index()
{
    return state.index;
}
