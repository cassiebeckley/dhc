#include "variable.hpp"

#include <iostream>
#include <unicode/ustream.h>

#include <cstdlib>

using namespace dhc::kernel::expression;

value::Value &Variable::evaluate()
{
    std::cerr << "Error: could not find variable '" << name << "'. Also, this probably should be an exception." << std::endl;
    exit(1);
}

expression_ptr Variable::bind(std::map<icu::UnicodeString, expression_ptr> env)
{
    try
    {
        return env.at(name);
    }
    catch (std::out_of_range &e)
    {
        return nullptr;
    }
}


dhc::kernel::type::Type Variable::type()
{
    return type::Type(std::vector<icu::UnicodeString> {});
}

icu::UnicodeString Variable::str()
{
    return name;
}
