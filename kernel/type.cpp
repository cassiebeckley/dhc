#include "type.hpp"

using namespace dhc::kernel::type;

icu::UnicodeString Type::str() const
{
    switch (this->type_type)
    {
        case VALUE:
        {
            icu::UnicodeString str;
            for (auto it = this->value.constructors.begin(); it != this->value.constructors.end(); ++it)
            {
                if (it != this->value.constructors.begin())
                    str += " | ";
                str += *it;
            }
            return str;
        }
        default:
            return "<WAT>";
    }
}
