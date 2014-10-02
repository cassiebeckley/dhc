#include "expression.hpp"
#include "expression/value.hpp"

// TODO: remove
#include <iostream>
#include <cstdlib>

using namespace dhc::kernel;
using namespace dhc::kernel::expression;
using namespace dhc::kernel::expression::value;

Expression::Expression(Value v)
{
    value.reset(new Value(v));
    expression_type = VALUE;
}

Expression::Expression(const Expression &other)
{
    switch (other.expression_type)
    {
        case APPLICATION:
            this->expression_type = APPLICATION;
            this->application = other.application;
            break;
        case CASE:
            this->expression_type = CASE;
            this->caseexp = other.caseexp;
            break;
        case VALUE:
            this->expression_type = VALUE;
            this->value.reset(new Value(*other.value));
            break;
        case VARIABLE:
            this->expression_type = VARIABLE;
            this->variable = std::make_pair(other.variable.first, std::unique_ptr<Expression>(new Expression(*other.variable.second)));
            break;
    }
}

value::Value Expression::evaluate() const
{
    switch(this->expression_type)
    {
        case APPLICATION:
        {
            Value fvalue = this->application.function->evaluate();
            if (fvalue.value_type != Value::FUNCTION)
            {
                std::cerr << "error: calling value of non-function type" << std::endl;
                exit(1);
            }

            fvalue.bind(fvalue.function.argument, *this->application.argument);
            return fvalue.function.exp->evaluate();
        }
        case CASE:
            for (auto it = this->caseexp.patterns.begin(); it != this->caseexp.patterns.end(); ++it)
            {
                pattern::Pattern p = it->first;
                Expression e = it->second;
                Maybe<std::map<icu::UnicodeString, Expression>> succeeded = p.test(*this->caseexp.exp);
                if (succeeded)
                {
                    std::map<icu::UnicodeString, Expression> vars = succeeded.data;
                    // add vars to the environment for e
                    for (auto it2 = vars.begin(); it2 != vars.end(); ++it2)
                    {
                        e.bind(it2->first, it2->second);
                    }

                    return e.evaluate();
                }
            }
            std::cerr << "No patterns matched in case expression. Also, I should probably use proper exceptions here." << std::endl;
            exit(1);
        case VALUE:
            return *this->value;
        case VARIABLE:
            if (this->variable.second != nullptr)
            {
                return this->variable.second->evaluate();
            }
            else
            {
                std::cerr << "Error: could not find variable '" << this->variable.first << "'. Also, this probably should be an exception." << std::endl;
                exit(1);
            }
        default:
            std::cerr << "Oh dear. An abnormality has occurred. Terminating." << std::endl;
            exit(1);
    }
}

type::Value Expression::type() const
{
    switch (this->expression_type)
    {
        case VALUE:
            return this->value->type();
        default:
            return type::Value(std::vector<icu::UnicodeString> {});
    }
}

icu::UnicodeString Expression::str() const
{
    switch (this->expression_type)
    {
        case APPLICATION:
        {
            return "(" + this->application.function->str() + " " + this->application.argument->str() + ")";
        case CASE:
            auto s = "(case " + this->caseexp.exp->str() + " of {";
            for (auto it = this->caseexp.patterns.begin(); it != this->caseexp.patterns.end(); ++it)
            {
                if (it != this->caseexp.patterns.begin())
                    s += "; ";
                s += it->first.str() + " -> " + it->second.str();
            }
            s += "})";
            return s;
        }
        case VALUE:
            return this->value->str();
        case VARIABLE:
            return this->variable.first;
        default:
            std::cerr << "Oh dear. An abnormality has occurred. Terminating." << std::endl;
            exit(1);
    }
}

void Expression::bind(icu::UnicodeString var, Expression val)
{
    switch (this->expression_type)
    {
        case APPLICATION:
            application.function->bind(var, val);
            application.argument->bind(var, val);
            break;
        case CASE:
            this->caseexp.exp->bind(var, val);
            for (auto it = this->caseexp.patterns.begin(); it != this->caseexp.patterns.end(); ++it)
            {
                it->second.bind(var, val);
            }
            break;
        case VALUE:
            this->value->bind(var, val);
            break;
        case VARIABLE:
            if (this->variable.first == var)
            {
                this->variable.second.reset(new Expression(val));
            }
            break;
    }
}
