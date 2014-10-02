#include "application.hpp"
#include "../expression.hpp"

using namespace dhc::kernel::expression;

Application::Application(const Expression &f, const Expression &a)
{
    function.reset(new Expression(f));
    argument.reset(new Expression(a));
}

Application &Application::operator=(const Application &other)
{
    function.reset(new Expression(*other.function));
    argument.reset(new Expression(*other.argument));
    return *this;
}
