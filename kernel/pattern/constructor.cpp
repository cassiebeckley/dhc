#include "constructor.hpp"

using namespace dhc::kernel::pattern;

MaybeEnv Constructor::test(dhc::kernel::expression::expression_ptr e)
{
    std::map<icu::UnicodeString, dhc::kernel::expression::expression_ptr> env;

    dhc::kernel::expression::value::Value &val = e->evaluate();

    if (val.constructor() == constructor)
    {
        for (unsigned int i = 0; i < fields.size(); i++)
        {
            auto pat = fields.at(i);
            auto exp = val.at(i);

            auto matched = pat->test(exp);
            if (matched)
            {
                auto extra_env = matched.data;
                env.insert(extra_env.begin(), extra_env.end());
            }
            else
            {
                return MaybeEnv();
            }
        }
    }
    else
    {
        return MaybeEnv();
    }

    return MaybeEnv(env);
}

icu::UnicodeString Constructor::str()
{
    icu::UnicodeString s = datatype.constructors[constructor];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        s += " " + (*it)->str();
    }

    return s;
}
