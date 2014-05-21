#include "variable.hpp"

#include <iostream>
#include <unicode/ustream.h>

#include <cstdlib>

using namespace dhc::kernel::expression;

value_ref Variable::evaluate() const
{
    try
    {
        auto exp = environment.at(name);
        return exp->evaluate();
    }
    catch (std::out_of_range &e)
    {
        std::cerr << "Error: could not find variable '" << name << "'. Also, this probably should be an exception." << std::endl;
        exit(1);
    }
}

void Variable::bind(std::map<icu::UnicodeString, expression_ptr> env) const
{
    environment.insert(env.begin(), env.end());
}

dhc::kernel::type::Type Variable::type() const
{
    return type::Type(std::vector<icu::UnicodeString> {});
}

icu::UnicodeString Variable::str() const
{
    return name;
}
