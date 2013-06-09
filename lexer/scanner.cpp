#include "scanner.hpp"

char dhc::lexer::scanner::peek()
{
    return source.at(state.index);
}

char dhc::lexer::scanner::get()
{
    state.column++;
    return source.at(state.index++);
}

dhc::lexer::scanstate dhc::lexer::scanner::get_state()
{
    return state;
}

void dhc::lexer::scanner::set_state(scanstate& s)
{
    state = s;
}

bool dhc::lexer::scanner::finished()
{
    return state.index >= source.length();
}

void dhc::lexer::scanner::newline()
{
    state.line_number++;
    state.column = 0;
}

unsigned int dhc::lexer::scanner::lineno()
{
    return state.line_number;
}

unsigned int dhc::lexer::scanner::charno()
{
    return state.column;
}
