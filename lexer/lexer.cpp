#include "lexer.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::lexer::lexer::next()
{
    if (!finished())
        return final_tokens[index++];
    else
        return nullptr;
}

std::shared_ptr<dhc::graft::match::match> dhc::lexer::lexer::generate_next(pattern_ptr program)
{
    if (!tokens.empty())
    {
        match_ptr m = tokens.front();
        tokens.pop();
        return m;
    }

    if (s.finished())
    {
        return nullptr;
    }

    previous_column = column;

    auto m = program->find(s);
    auto str = m->flatten();

    column += s.index - previous_index;
    previous_index = s.index;

    if (m->type == static_cast<int>(type::WHITESPACE))
    {
        return generate_next(program);
    }
    else if (beginning)
    {
        beginning = false;
        if (str != "{" && str != "module")
        {
            unsigned int n = previous_column + 1;
            tokens.push(m);
            current_line = line_number;
            return std::make_shared<integer>(static_cast<int>(type::CURLY), 1, n);
        }
    }
    else if (expecting)
    {
        expecting = false;
        if (str != "{")
        {
            unsigned int n = previous_column + 1;
            tokens.push(m);
            current_line = line_number;
            return std::make_shared<integer>(static_cast<int>(type::CURLY), 1, n);
        }
    }
    else if(m->type == static_cast<int>(type::RESERVEDID) && (str == "let" || str == "where" || str == "do" || str == "of"))
    {
        expecting = true;
    }

    if (line_number > current_line)
    {
        current_line = line_number;

        unsigned int n = previous_column + 1;

        tokens.push(m);
        return std::make_shared<integer>(static_cast<int>(type::ANGLE), 1, n);
    }

    return m;
}

bool dhc::lexer::lexer::finished()
{
    //std::cout << "Index: " << index << " size: " << final_tokens.size() << std::endl;
    return index >= final_tokens.size();
}

std::string dhc::lexer::lexer::error(std::string filename)
{
    std::stringstream ss;
    ss << filename << ':' << line_number << ':' << column << ": Lexical error" << std::endl;
    return ss.str();
}
