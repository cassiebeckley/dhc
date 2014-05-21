#include "ignored.hpp"

using namespace dhc::kernel::pattern;

MaybeEnv Ignored::test(dhc::kernel::expression::expression_ptr)
{
    std::map<icu::UnicodeString, dhc::kernel::expression::expression_ptr> env;
    return MaybeEnv(env);
}

std::set<icu::UnicodeString> Ignored::matches()
{
    std::set<icu::UnicodeString> m;
    return m;
}

icu::UnicodeString Ignored::str()
{
    return "_";
}
