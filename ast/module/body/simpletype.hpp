#ifndef DHC_AST_SIMPLETYPE_HPP
#define DHC_AST_SIMPLETYPE_HPP

#include <unicode/unistr.h>
#include <vector>

namespace dhc
{
    namespace ast
    {
        class simpletype
        {
            public:
                icu::UnicodeString tycon;
                std::vector<icu::UnicodeString> tyvars;

            protected:
            private:
        }
    }
}

#endif
