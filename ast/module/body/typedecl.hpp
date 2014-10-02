#ifndef DHC_AST_TYPEDECL_HPP
#define DHC_AST_TYPEDECL_HPP

#include <unicode/unistr.h>
#include <vector>

namespace dhc
{
    namespace ast
    {
        /**
         * \brief Declares a type synonym
         */
        class type
        {
            public:
                simpletype s;
                type t;
            protected:
            private:
        }
    }
}

#endif
