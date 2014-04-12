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

#include <iostream>
#include <unicode/ustream.h>

int main()
{
    using namespace dhc::kernel::expression;
    using namespace dhc::kernel::expression::value;
    std::cout << "Hey." << std::endl;

    dhc::kernel::type::Type Three(std::vector<icu::UnicodeString>{"One", "Two", "Three"});
    dhc::kernel::type::Type Bool(std::vector<icu::UnicodeString>{"False", "True"});

    auto pat1 = std::make_shared<dhc::kernel::pattern::Constructor>(true, std::vector<dhc::kernel::expression::pattern_ptr>{}, Bool);
    auto pat1exp = std::make_shared<UserValue>(2, std::vector<expression_ptr>{}, Three);

    auto pat2 = std::make_shared<dhc::kernel::pattern::Constructor>(false, std::vector<dhc::kernel::expression::pattern_ptr>{}, Bool);
    auto pat2exp = std::make_shared<UserValue>(0, std::vector<expression_ptr>{}, Three);

    auto c = std::make_shared<Case>(std::make_shared<Variable>("x"), std::vector<std::pair<pattern_ptr, expression_ptr>>{std::make_pair(pat1, pat1exp), std::make_pair(pat2, pat2exp)});

    auto f = std::make_shared<Function>("x", c);

    auto a = std::make_shared<Application>(f, std::make_shared<UserValue>(false, std::vector<expression_ptr>{}, Bool));

    std::cout << a->str() << std::endl;
    std::cout << a->evaluate().str() << std::endl;

    return 0;
}

