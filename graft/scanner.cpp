#include "scanner.hpp"

std::shared_ptr<dhc::graft::match::character> dhc::graft::scanner::next()
{
    return std::make_shared<match::character>(state.column++, -1, source.at(state.index++));
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
    return state.index >= source.length();
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
