#ifndef DHC_AST_CLASS_ASSERTION_HPP
#define DHC_AST_CLASS_ASSERTION_HPP

#include <unicode/unistr.h>
#include <vector>

namespace dhc
{
    namespace ast
    {
        class class_assertion
        {
            public:
                icu::UnicodeString class_identifier;
                icu::UnicodeString type_variable;
                std::vector<icu::UnicodeString> types;
            protected:
            private:
        }
    }
}

#endif
