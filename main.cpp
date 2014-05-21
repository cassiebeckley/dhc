#include "kernel/expression.hpp"
#include "kernel/expression/application.hpp"
#include "kernel/expression/case.hpp"
#include "kernel/expression/variable.hpp"

#include "kernel/expression/value.hpp"
#include "kernel/expression/value/uservalue.hpp"
#include "kernel/expression/value/function.hpp"

#include "kernel/pattern.hpp"
#include "kernel/pattern/variable.hpp"
#include "kernel/pattern/constructor.hpp"
#include "kernel/pattern/ignored.hpp"

#include <iostream>
#include <unicode/ustream.h>

int main()
{
    using namespace dhc::kernel::expression;
    using namespace dhc::kernel::expression::value;

    dhc::kernel::type::Type Three(std::vector<icu::UnicodeString>{"One", "Two", "Three"});
    dhc::kernel::type::Type Bool(std::vector<icu::UnicodeString>{"False", "True"});
    dhc::kernel::type::Type Maybe(std::vector<icu::UnicodeString>{"Just", "Nothing"});
    dhc::kernel::type::Type List(std::vector<icu::UnicodeString>{"Cons", "Nil"});

    std::map<icu::UnicodeString, expression_ptr> env;

    auto head = std::make_shared<Function>("_a", std::make_shared<Case>(std::make_shared<Variable>("_a"), std::vector<std::pair<pattern_ptr, expression_ptr>>{std::make_pair(std::make_shared<dhc::kernel::pattern::Constructor>(0, std::vector<dhc::kernel::expression::pattern_ptr>{std::make_shared<dhc::kernel::pattern::Variable>("x"), std::make_shared<dhc::kernel::pattern::Ignored>()}, List), std::make_shared<Variable>("x"))}));

    env["head"] = head;

    auto tail = std::make_shared<Function>("_a", std::make_shared<Case>(std::make_shared<Variable>("_a"), std::vector<std::pair<pattern_ptr, expression_ptr>>{std::make_pair(std::make_shared<dhc::kernel::pattern::Constructor>(0, std::vector<dhc::kernel::expression::pattern_ptr>{std::make_shared<dhc::kernel::pattern::Ignored>(), std::make_shared<dhc::kernel::pattern::Variable>("xs")}, List), std::make_shared<Variable>("xs"))}));

    env["tail"] = tail;

    auto repeat = std::make_shared<Function>("x", std::make_shared<UserValue>(0, std::vector<expression_ptr>{std::make_shared<Variable>("x"), std::make_shared<Application>(std::make_shared<Variable>("repeat"), std::make_shared<Variable>("x"))}, List));

    env["repeat"] = repeat;

    auto bleh = std::make_shared<UserValue>(0, std::vector<expression_ptr>{std::make_shared<UserValue>(0, std::vector<expression_ptr>{}, Three), std::make_shared<UserValue>(0, std::vector<expression_ptr>{std::make_shared<UserValue>(1, std::vector<expression_ptr>{}, Three), std::make_shared<UserValue>(0, std::vector<expression_ptr>{std::make_shared<UserValue>(2, std::vector<expression_ptr>{}, Three), std::make_shared<UserValue>(1, std::vector<expression_ptr>{}, List)}, List)}, List)}, List);

    env["bleh"] = bleh;

    auto main = std::make_shared<Application>(std::make_shared<Variable>("head"), std::make_shared<Application>(std::make_shared<Variable>("repeat"), std::make_shared<UserValue>(0, std::vector<expression_ptr>{}, Three)));

    env["main"] = main;

    std::cout << """Printing environment""" << std::endl;
    for (auto it = env.begin(); it != env.end(); ++it)
    {
        it->second->bind(env);
        std::cout << it->first << " = " << it->second->str() << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Evaluating main" << std::endl;

    std::cout << env["main"]->evaluate().str() << std::endl;

    return 0;
}

