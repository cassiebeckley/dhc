#include "lexer.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::lexer::lexer::next()
{
    if (s.finished())
        return nullptr;

    return program->find(s);
}

bool dhc::lexer::lexer::finished()
{
    return s.finished();
}

std::string dhc::lexer::lexer::error(std::string filename)
{
    std::stringstream ss;
    ss << filename << /*':' << s.lineno() << ':' << s.charno() << */ ": Lexical error" << std::endl;
    return ss.str();
}
