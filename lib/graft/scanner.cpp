#include "scanner.hpp"

dhc::graft::scanner::~scanner()
{
    delete [] source;
}

std::shared_ptr<dhc::graft::match::character> dhc::graft::scanner::next()
{
    if (!finished())
    {
        return std::make_shared<match::character>(state.column++, -1, source[state.index++]);
    }

    return nullptr;
}

dhc::graft::scanstate dhc::graft::scanner::get_state()
{
    return state;
}

void dhc::graft::scanner::set_state(scanstate& s)
{
    state = s;
}

bool dhc::graft::scanner::finished()
{
    //std::cout << "Finished yet? state.index: " << state.index << " length: " << length << std::endl;
    return state.index >= length;
}

void dhc::graft::scanner::newline()
{
    state.line_number++;
    state.column = 0;
}

unsigned int dhc::graft::scanner::lineno()
{
    return state.line_number;
}

unsigned int dhc::graft::scanner::charno()
{
    return state.column;
}

void dhc::graft::scanner::initialize(icu::UnicodeString src)
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
