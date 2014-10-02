#ifndef DHC_AST_CLASSDECL_HPP
#define DHC_AST_CLASSDECL_HPP

#include "class_assertion.hpp"

#include "fixitydecl.hpp"
#include "typesig.hpp"
#include "vardecl.hpp"

namespace dhc
{
    namespace ast
    {
        class classdecl
        {
            public:
                std::vector<class_assertion> context;
                icu::UnicodeString tycls;
                icu::UnicodeString tyvar;

                std::vector<fixitydecl> fixitydecls;
                std::vector<typesig> typesigs;
                std::vector<vardecl> vardecls;
            protected:
            private:
        }
    }
}

#endif
