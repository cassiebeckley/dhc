#include "function.hpp"
#include "../../type.hpp"
#include "../../expression.hpp"

#include <cstdlib>

using namespace dhc::kernel::expression::value;

Function::Function(icu::UnicodeString a, const expression::Expression &e)
{
    argument = a;
    exp.reset(new expression::Expression(e));
}

Function::Function(Function &other)
{
    this->argument = other.argument;
    this->exp.reset(*(other.exp));
}

Function &Function::operator=(const Function &other)
{
    argument = other.argument;
    exp.reset(new expression::Expression(other.exp));
    return *this;
}

/*
dhc::kernel::expression::value_ptr Function::evaluate() const
{
    return value_ptr(this);
}

void Function::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    exp->bind(environment);
}

dhc::kernel::expression::type_ptr Function::type() const
{
    return exp->type();
}

dhc::kernel::expression::expression_ptr Function::apply(expression_ptr e) const
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
*/
