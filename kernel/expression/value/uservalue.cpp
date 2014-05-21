#include "uservalue.hpp"

using namespace dhc::kernel::expression::value;

dhc::kernel::expression::value_ref UserValue::evaluate() const
{
    return *this;
}

void UserValue::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        (*it)->bind(environment);
    }
}

dhc::kernel::type::Type UserValue::type() const
{
    return datatype;
}

unsigned int UserValue::constructor() const
{
    return ctr;
}

dhc::kernel::expression::expression_ptr UserValue::at(int i) const
{
    auto exp = fields.at(i);
    return exp;
}

icu::UnicodeString UserValue::str() const
{
    icu::UnicodeString v = "(" + datatype.constructors[ctr];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        v += " " + (*it)->str();
    }

    v += ")";

    return v;
}
