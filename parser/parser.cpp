#include "parser.hpp"
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::parser::parser::next()
{
    if (lex.finished())
        return nullptr;

    return final->find(lex);
}

bool dhc::parser::parser::finished()
{
    return lex.finished();
}

std::string dhc::parser::parser::error(std::string filename)
{
    std::stringstream ss;
    ss << filename << ':' << lex.lineno() << ':' << lex.charno() << ": Parse error" << std::endl;
    return ss.str();
}
