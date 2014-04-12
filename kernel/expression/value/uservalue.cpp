#include "uservalue.hpp"

using namespace dhc::kernel::expression::value;

Value &UserValue::evaluate()
{
    return *this;
}

dhc::kernel::expression::expression_ptr UserValue::bind(std::map<icu::UnicodeString, expression_ptr> env)
{
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        auto ftemp = (*it)->bind(env);
        if (ftemp)
            *it = ftemp;
    }

    return nullptr;
}


dhc::kernel::type::Type UserValue::type()
{
    return datatype;
}

unsigned int UserValue::constructor()
{
    return ctr;
}

dhc::kernel::expression::expression_ptr UserValue::at(int i)
{
    return fields.at(i);
}

icu::UnicodeString UserValue::str()
{
    icu::UnicodeString v = datatype.constructors[ctr];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        v += " " + (*it)->str();
    }

    return v;
}
