#include "application.hpp"
#include "value/function.hpp"

using namespace dhc::kernel::expression;

value::Value &Application::evaluate()
{
    auto f = static_cast<value::Function&>(function->evaluate());
    auto result = f.apply(argument);
    return result->evaluate();
}

expression_ptr Application::bind(std::map<icu::UnicodeString, expression_ptr> env)
{
    expression_ptr ftemp = function->bind(env);
    expression_ptr atemp = argument->bind(env);

    if (ftemp)
        function = ftemp;
    if (atemp)
        argument = atemp;

    return nullptr;
}


dhc::kernel::type::Type Application::type()
{
    return type::Type(std::vector<icu::UnicodeString> {});
}

icu::UnicodeString Application::str()
{
    return "(" + function->str() + " " + argument->str() + ")";
}
