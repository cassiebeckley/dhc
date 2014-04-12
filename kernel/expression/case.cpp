#include "case.hpp"
#include "../maybe.hpp"
#include <unicode/unistr.h>
#include <iostream>

#include <cstdlib>

using namespace dhc::kernel::expression;

value::Value &Case::evaluate()
{
    for (auto it = patterns.begin(); it != patterns.end(); ++it)
    {
        pattern_ptr p = it->first;
        expression_ptr e = it->second;

        Maybe<std::map<icu::UnicodeString, expression_ptr>> succeeded = p->test(exp);
        if (succeeded)
        {
            auto vars = succeeded.data;
            expression_ptr etemp = e->bind(vars);
            if (etemp)
                e = etemp;
            return e->evaluate();
        }
    }

    std::cerr << "No patterns matched in case expression. Also, I should probably use proper exceptions here." << std::endl;
    exit(1);
}

expression_ptr Case::bind(std::map<icu::UnicodeString, expression_ptr> env)
{
    auto exptemp = exp->bind(env);
    if (exptemp)
        exp = exptemp;

    for (auto it = patterns.begin(); it != patterns.end(); ++it)
    {
        expression_ptr temp = it->second->bind(env);
        if (temp)
            it->second = temp;
    }

    return nullptr;
}


dhc::kernel::type::Type Case::type()
{
    return type::Type(std::vector<icu::UnicodeString>{});
}

icu::UnicodeString Case::str()
{
    auto s = "(case " + exp->str() + " of {";
    for (auto it = patterns.begin(); it != patterns.end(); ++it)
    {
        if (it != patterns.begin())
            s += "; ";
        s += (*it).first->str() + " -> " + (*it).second->str();
    }
    s += "})";
    return s;
}
