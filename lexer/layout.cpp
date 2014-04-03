#include "layout.hpp"

#include <iostream>
#include <unicode/ustream.h>


// TODO: remove
#include <iostream>

std::shared_ptr<dhc::graft::match::match> dhc::lexer::layout::next()
{
    if (!finished())
    {
        if (index + 1 > max_index)
            max_index = index;

        return final_tokens[index++];
    }
    else
        return nullptr;
}

unsigned int dhc::lexer::layout::reset()
{
    std::cout << "Inserting '}' before \"" << final_tokens[max_index]->flatten() << "\", max_index: " << max_index << std::endl;

    index = 0;

    auto it = final_tokens.insert(final_tokens.begin() + max_index, std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '}'));

    for (++it; it != final_tokens.end(); ++it)
    {
        if ((*it)->flatten() == "}")
        {
            final_tokens.erase(it);
            return max_index;
        }
    }

    return false;
}

std::shared_ptr<dhc::graft::match::match> dhc::lexer::layout::generate_next()
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
            return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), ';');
        }
        else if (n < top)
        {
            context.pop();
            angle_tokens.push(n);
            return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '}');
        }
    }

    if (lex.finished())
    {

        if (!context.empty())
        {
            context.pop();
            return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '}');
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
            return generate_next();
        }

        m = generate_next();
    }
    else if (m->type == static_cast<int>(type::CURLY))
    {
        auto curly = std::static_pointer_cast<integer>(m);
        int n = curly->data;
        std::cout << '{' << n << '}' << std::endl;

        if (context.empty())
        {
            std::cout << "context is empty, pushing " << n << std::endl;
            context.push(n);
            return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '{');
        }
        else
        {
            int top = context.top();

            if (n > top)
            {
                std::cout << "innermost context is " << top << ", pushing " << n << std::endl;
                context.push(n);
                return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '{');
            }
            else
            {
                tokens.push(std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '}'));
                angle_tokens.push(n);
                return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '{');
            }
        }

        m = generate_next();
    }
    else if (m->type == static_cast<int>(type::SPECIAL))
    {
        icu::UnicodeString str = m->flatten();
        if (str == "}")
        {
            std::cout << "str is '}', context.top() is " << context.top() << std::endl;
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
            std::cout << "str is '{', pushing 0" << std::endl;
            context.push(0);
        }
    }
    else if (m->type == static_cast<int>(type::RESERVEDID) && m->flatten() == "in" && context.top() != 0)
    {
        context.pop();
        tokens.push(m);
        std::cout << "returning '}' before \"in\"" << std::endl;
        return std::make_shared<graft::match::character>(static_cast<int>(type::SPECIAL), '}');
    }

    return m;
}

bool dhc::lexer::layout::finished()
{
    return index >= final_tokens.size();
}
