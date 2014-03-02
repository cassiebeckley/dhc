#include "scanner.hpp"

dhc::graft::scanner::~scanner()
{
    delete [] source;
}

std::shared_ptr<dhc::graft::match::match> dhc::graft::scanner::next()
{
    UChar32 c = -1;

    if (!finished())
        c = source[state.index++];

    return std::shared_ptr<match::match> (new match::character(state.column++, -1, c));
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
