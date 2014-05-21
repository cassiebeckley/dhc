#include "application.hpp"
#include "value/function.hpp"

using namespace dhc::kernel::expression;

value_ref Application::evaluate() const
{
    auto f = static_cast<const value::Function&>(function->evaluate());
    auto result = f.apply(argument);
    return result->evaluate();
}

void Application::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    function->bind(environment);
    argument->bind(environment);
}

dhc::kernel::type::Type Application::type() const
{
    return type::Type(std::vector<icu::UnicodeString> {});
}

icu::UnicodeString Application::str() const
{
    return "(" + function->str() + " " + argument->str() + ")";
}
