#include "scanner.hpp"

std::shared_ptr<dhc::lexer::match::match> dhc::lexer::scanner::lookahead()
{
    return std::shared_ptr<match::match> (new match::character(state.column, -1, source.at(state.index)));
}

void dhc::lexer::scanner::consume()
{
    state.column++;
    state.index++;
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
