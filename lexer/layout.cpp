#include "layout.hpp"

std::shared_ptr<dhc::graft::match::match> dhc::lexer::layout::next()
{
    if (!tokens.empty())
    {
        auto m = tokens.front();
        //std::cout << "Type of front of queue: " << m->type << " is ANGLE? " << (m->type == static_cast<int>(type::ANGLE)) << m->type << " is CURLY? " << (m->type == static_cast<int>(type::CURLY)) << std::endl;
        tokens.pop();
        return m;
    }

    if (!angle_tokens.empty())
    {
        int top = context.top();
        int n = angle_tokens.front();
        angle_tokens.pop();

        if (top == n)
        {
            return std::make_shared<graft::match::character>(top, static_cast<int>(type::SPECIAL), ';');
        }
        else if (n < top)
        {
            context.pop();
            angle_tokens.push(n);
            return std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '}');
        }
    }

    if (lex.finished())
    {

        if (!context.empty())
        {
            int n = context.top();
            context.pop();
            return std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '}');
        }
        else
            return nullptr;
    }

    auto m = lex.next();

    if (m->type == static_cast<int>(type::ANGLE))
    {
        auto angle = std::static_pointer_cast<integer>(m);
        int n = angle->data;
        if (!context.empty())
        {
            angle_tokens.push(n);
            return next();
        }

        m = next();
    }
    else if (m->type == static_cast<int>(type::CURLY))
    {
        auto curly = std::static_pointer_cast<integer>(m);
        int n = curly->data;

        if (context.empty())
        {
            context.push(n);
            return std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '{');
        }
        else
        {
            int top = context.top();

            if (n > top)
            {
                context.push(n);
                return std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '{');
            }
            else
            {
                tokens.push(std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '}'));
                angle_tokens.push(n);
                return std::make_shared<graft::match::character>(n, static_cast<int>(type::SPECIAL), '{');
            }
        }

        m = next();
    }
    else if (m->type == static_cast<int>(type::SPECIAL))
    {
        icu::UnicodeString str = m->flatten();
        if (str == "}")
        {
            if (context.top() == 0)
            {
                context.pop();
            }
            else
            {
                return nullptr;
            }
        }
        else if (str == "{")
        {
            context.push(0);
        }
    }

    return m;
}

bool dhc::lexer::layout::finished()
{
    return lex.finished() && context.empty();
}

dhc::graft::scanner::scanstate &dhc::lexer::layout::state()
{
    return lex.state();
}
