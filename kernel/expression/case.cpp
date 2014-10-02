#include "case.hpp"
#include "../expression.hpp"

using namespace dhc::kernel::expression;

Case::Case(const Case &other)
{
    this->exp.reset(new Expression(*(other.exp)));

    for (auto it = other.patterns.begin(); it != other.patterns.end(); ++it)
    {
        this->patterns.push_back(*it);
    }
}

Case::Case(const Expression &e, const std::vector<std::pair<pattern::Pattern, Expression>> p)
{
    this->exp.reset(new Expression(e));

    for (auto it = p.begin(); it != p.end(); ++it)
    {
        this->patterns.push_back(*it);
    }
}

Case &Case::operator=(const Case &other)
{
    this->exp.reset(new Expression(*(other.exp)));

    for (auto it = other.patterns.begin(); it != other.patterns.end(); ++it)
    {
        this->patterns.push_back(*it);
    }

    return *this;
}
