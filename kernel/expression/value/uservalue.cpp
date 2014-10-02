#include "uservalue.hpp"
#include "../../expression.hpp"

using namespace dhc::kernel::expression::value;

UserValue::UserValue(unsigned int ctr, std::vector<Expression> fields, type::Value datatype)
{
    this->ctr = ctr;

    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        this->fields.push_back(std::unique_ptr<Expression>(new Expression(*it)));
    }

    this->datatype = datatype;
}

UserValue::UserValue(const UserValue &other)
{
    this->ctr = other.ctr;

    for (auto it = other.fields.begin(); it != other.fields.end(); ++it)
    {
        this->fields.push_back(std::unique_ptr<Expression>(new Expression(**it)));
    }

    this->datatype = other.datatype;
}

/*
dhc::kernel::expression::value_ptr UserValue::evaluate() const
{
    return value_ptr(this);
}

void UserValue::bind(std::map<icu::UnicodeString, expression_ptr> environment) const
{
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        (*it)->bind(environment);
    }
}

dhc::kernel::expression::type_ptr UserValue::type() const
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
    icu::UnicodeString v = "(" + datatype->constructors[ctr];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        v += " " + (*it)->str();
    }

    v += ")";

    return v;
}
*/
