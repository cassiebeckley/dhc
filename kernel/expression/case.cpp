#include "case.hpp"
#include "../maybe.hpp"
#include <unicode/unistr.h>
#include <iostream>

#include <cstdlib>

using namespace dhc::kernel::expression;

value_ref Case::evaluate() const
{
    for (auto it = patterns.begin(); it != patterns.end(); ++it)
    {
        pattern_ptr p = it->first;
        expression_ptr e = it->second;

        Maybe<std::map<icu::UnicodeString, expression_ptr>> succeeded = p->test(exp);
        if (succeeded)
        {
            auto vars = succeeded.data;
            e->bind(vars);
            const value::Value& bleh = e->evaluate();
            return bleh;
        }
    }

    std::cerr << "No patterns matched in case expression. Also, I should probably use proper exceptions here." << std::endl;
    exit(1);
}

void Case::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    exp->bind(environment);
    for (auto it = patterns.begin(); it != patterns.end(); ++it)
    {
        expression_ptr e = it->second;
        e->bind(environment);
    }
}

dhc::kernel::type::Type Case::type() const
{
    return type::Type(std::vector<icu::UnicodeString>{});
}

icu::UnicodeString Case::str() const
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
