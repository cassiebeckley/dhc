#include "function.hpp"

#include <cstdlib>

using namespace dhc::kernel::expression::value;

Value &Function::evaluate()
{
    return *this;
}

dhc::kernel::expression::expression_ptr Function::bind(std::map<icu::UnicodeString, expression_ptr> env)
{
    expression_ptr etemp = exp->bind(env);
    if (etemp)
        exp = etemp;

    return nullptr;
}


dhc::kernel::type::Type Function::type()
{
    return exp->type();
}

dhc::kernel::expression::expression_ptr Function::apply(expression_ptr e)
{
    std::map<icu::UnicodeString, expression_ptr> env;
    env[argument] = e;

    expression_ptr etemp = exp->bind(env);
    if (etemp)
        exp = etemp;

    return exp;
}

unsigned int Function::constructor()
{
    // Not really sure what to do here >.>
    return -1;
}

dhc::kernel::expression::expression_ptr Function::at(int)
{
    throw std::out_of_range("Can't access fields of function value");
}

icu::UnicodeString Function::str()
{
    return "(\\" + argument + " -> " + exp->str() + ")";
}
