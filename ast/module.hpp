#ifndef DHC_AST_MODULE_HPP
#define DHC_AST_MODULE_HPP

#include "../kernel/expression.hpp"
#include "../kernel/expression/value.hpp"
#include <unicode/unistr.h>

namespace dhc
{
    namespace ast
    {
        namespace module
        {

            class module
            {
                public:
                    std::map<icu::UnicodeString, dhc::kernel::expression::Expression> load();
            };
        }
    }
}

#endif
