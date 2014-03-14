#include "lexer.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::lexer::lexer::next()
{
    if (!tokens.empty())
    {
        match_ptr m = tokens.front();
        tokens.pop();
        return m;
    }

    if (s.finished())
        return nullptr;

    auto m = program->find(s);
    auto str = m->flatten();

    if (m->type == static_cast<int>(type::WHITESPACE))
    {
        return next();
    }
    else if (beginning)
    {
        beginning = false;
        if (str != "{" && str != "module")
        {
            unsigned int n = m->column + 1;
            tokens.push(m);
            current_line = state_line_number();
            return std::make_shared<integer>(m->column, static_cast<int>(type::CURLY), 1, n);
        }
    }
    else if (expecting)
    {
        expecting = false;
        if (str != "{")
        {
            unsigned int n = m->column + 1;
            tokens.push(m);
            current_line = state_line_number();
            return std::make_shared<integer>(m->column, static_cast<int>(type::CURLY), 1, n);
        }
    }
    else if(m->type == static_cast<int>(type::RESERVEDID) && (str == "let" || str == "where" || str == "do" || str == "of"))
    {
        expecting = true;
    }

    if (state_line_number() > current_line)
    {
        current_line = s.lineno();

        unsigned int n = m->column + 1;

        tokens.push(m);
        return std::make_shared<integer>(m->column, static_cast<int>(type::ANGLE), 1, n);
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
