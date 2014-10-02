#ifndef DHC_AST_DATA_HPP
#define DHC_AST_DATA_HPP

#include "class_assertion.hpp"
#include "simpletype.hpp"
#include <unicode/unistr.h>
#include <vector>

namespace dhc
{
    namespace ast
    {
        /**
         * \brief Represents an algebraic datatype declaration.
         */
        class data
        {
            public:
                std::vector<class_assertion> context;
                simpletype s;
                std::vector<constr> constrs;
            protected:
            private:
        }
    }
}

#endif
