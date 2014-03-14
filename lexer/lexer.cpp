#include "lexer.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::lexer::lexer::next()
{
    if (s.finished())
        return nullptr;

    auto m = program->find(s);

    if (m->type == static_cast<int>(type::WHITESPACE))
    {
        return nullptr;
    }

    return m;
}

bool dhc::lexer::lexer::finished()
{
    return s.finished();
}

dhc::graft::scanner::scanstate &dhc::lexer::lexer::state()
{
    return s.state();
}

std::string dhc::lexer::lexer::error(std::string filename)
{
    std::stringstream ss;
    ss << filename << ':' << s.lineno() << ':' << s.charno() << ": Lexical error" << std::endl;
    return ss.str();
}
