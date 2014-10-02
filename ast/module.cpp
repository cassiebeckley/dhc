#include "module.hpp"

using namespace dhc::ast::module;
using namespace dhc::kernel;
using namespace dhc::kernel::expression;
using namespace dhc::kernel::expression::value;

std::map<icu::UnicodeString, Expression> module::load()
{

    type::Value Three(std::vector<icu::UnicodeString>{"One", "Two", "Three"});
    type::Value Bool(std::vector<icu::UnicodeString>{"False", "True"});
    type::Value Maybe(std::vector<icu::UnicodeString>{"Just", "Nothing"});
    type::Value List(std::vector<icu::UnicodeString>{"Cons", "Nil"});

    std::map<icu::UnicodeString, Expression> env;

    Function head
    (
        "_a",
        Case
        (
            Expression("_a"),
            std::vector<std::pair<pattern::Pattern, Expression>>
            {
                std::make_pair
                (
                    pattern::Constructor
                    (
                        0,
                        std::vector<pattern::Pattern>
                        {
                            pattern::Pattern("x"),
                            pattern::Pattern()
                        },
                        List
                    ),
                    Expression("x")
                )
            }
        )
    );

    env.emplace("head", Expression(head));

    Function tail
    (
        "_a",
        Case
        (
            Expression("_a"),
            std::vector<std::pair<pattern::Pattern, Expression>>
            {
                std::make_pair
                (
                    pattern::Constructor
                    (
                        0,
                        std::vector<pattern::Pattern>
                        {
                            pattern::Pattern(),
                            pattern::Pattern("xs")
                        },
                        List
                    ),
                    Expression("xs")
                )
            }
        )
    );

    env.emplace("tail", Expression(tail));

    Function repeat
    (
        "x",
        Value(UserValue(0, std::vector<Expression>
        {
            Expression("x"),
            Application
            (
                Expression("repeat"),
                Expression("x")
            )
        }, List))
    );

    env.emplace("repeat", Expression(repeat));

    UserValue bleh
    (
        0,
        std::vector<Expression>
        {
            Value(UserValue(0, std::vector<Expression>{}, Three)),
            Value(UserValue(0, std::vector<Expression>
            {
                Value(UserValue(1, std::vector<Expression>{}, Three)),
                Value(UserValue(0, std::vector<Expression>
                {
                    Value(UserValue(2, std::vector<Expression>{}, Three)),
                    Value(UserValue(1, std::vector<Expression>{}, List))
                }, List))
            }, List))
        }, List
    );

    env.emplace("bleh", Expression(bleh));

    Application main
    (
        Expression("tail"),
        Application
        (
            Expression("repeat"),
            Value(UserValue(0, std::vector<Expression>{}, Three))
        )
    );

    env.emplace("main", main);

    return env;
} 
