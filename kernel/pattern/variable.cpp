#include "variable.hpp"

using namespace dhc::kernel::pattern;

MaybeEnv Variable::test(dhc::kernel::expression::expression_ptr e)
{
    std::map<icu::UnicodeString, dhc::kernel::expression::expression_ptr> env;
    env[name] = e;
    return MaybeEnv(env);
}

std::set<icu::UnicodeString> Variable::matches()
{
    std::set<icu::UnicodeString> m;
    m.insert(name);
    return m;
}

icu::UnicodeString Variable::str()
{
    return name;
}
