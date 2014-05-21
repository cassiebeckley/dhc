#include "function.hpp"

#include <cstdlib>

using namespace dhc::kernel::expression::value;

dhc::kernel::expression::value_ref Function::evaluate() const
{
    return *this;
}

void Function::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    exp->bind(environment);
}

dhc::kernel::type::Type Function::type() const
{
    return exp->type();
}

dhc::kernel::expression::expression_ptr Function::apply(expression_ptr e)
{
    std::map<icu::UnicodeString, expression_ptr> env;
    env[argument] = e;

    exp->bind(env);

    return exp;
}

unsigned int Function::constructor() const
{
    // Not really sure what to do here >.>
    return -1;
}

dhc::kernel::expression::expression_ptr Function::at(int) const
{
    throw std::out_of_range("Can't access fields of function value");
}

icu::UnicodeString Function::str() const
{
    return "(\\" + argument + " -> " + exp->str() + ")";
}
