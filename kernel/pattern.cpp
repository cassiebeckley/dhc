#include "pattern.hpp"

using namespace dhc::kernel::pattern;

Pattern::Pattern(const Pattern &other)
{
    this->pattern_type = other.pattern_type;
    switch (other.pattern_type)
    {
        case CONSTRUCTOR:
            this->constructor = other.constructor;
            break;
        case VARIABLE:
            this->variable = other.variable;
    }
}
