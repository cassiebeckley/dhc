#include "scanner.hpp"

void dhc::graft::scanner::scanner::newline()
{
    state_line_number()++;
    state_column() = 0;
}

unsigned int &dhc::graft::scanner::scanner::state_line_number()
{
    return state.line_number;
}

unsigned int &dhc::graft::scanner::scanner::state_column()
{
    return state.column;
}

unsigned int &dhc::graft::scanner::scanner::state_index()
{
    return state.index;
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
