#ifndef DHC_KERNEL_EXPRESSION_CASE_HPP
#define DHC_KERNEL_EXPRESSION_CASE_HPP

#include <memory>
#include <vector>

#include <unicode/ustream.h>

#include "../pattern.hpp"

namespace dhc {
    namespace kernel {

        namespace pattern {
            class Pattern;
        }

        namespace expression {
            
            class Expression;

            /**
             * Represents a Haskell case expression
             */
            struct Case
            {
                Case(const Expression &e, std::vector<std::pair<pattern::Pattern, Expression>> p);
                Case(const Case &other);

                Case &operator=(const Case &other);

                std::unique_ptr<Expression> exp;
                std::vector<std::pair<pattern::Pattern, Expression>> patterns;
            };
        }
    }
}

#endif
