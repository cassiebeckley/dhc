#include "value.hpp"

using namespace dhc::kernel::expression::value;
using namespace dhc::kernel;

Value::Value (const Value &other)
{
    switch (other.value_type)
    {
        case FUNCTION:
            this->value_type = FUNCTION;
            this->function = other.function;
            break;
        case USERVALUE:
            this->value_type = USERVALUE;
            this->uservalue = other.uservalue;
            break;
    }
}

Value &Value::operator=(const Value &other)
{
    switch (other.value_type)
    {
        case FUNCTION:
            this->value_type = FUNCTION;
            this->function = other.function;
            break;
        case USERVALUE:
            this->value_type = USERVALUE;
            this->uservalue = other.uservalue;
            break;
    }

    return *this;
}

type::Value Value::type() const
{
    switch (this->value_type)
    {
        case FUNCTION:
            return type::Value();
            break;
        case USERVALUE:
            return this->uservalue.datatype;
            break;
    }

    return type::Value();
}
