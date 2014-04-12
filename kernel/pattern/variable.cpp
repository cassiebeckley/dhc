#include "variable.hpp"

using namespace dhc::kernel::pattern;

MaybeEnv Variable::test(dhc::kernel::expression::expression_ptr e)
{
    std::map<icu::UnicodeString, dhc::kernel::expression::expression_ptr> env;
    env[name] = e;
    return MaybeEnv(env);
}

icu::UnicodeString Variable::str()
{
    return name;
}
