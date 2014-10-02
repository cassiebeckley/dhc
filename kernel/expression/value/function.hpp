#ifndef DHC_KERNEL_EXPRESSION_VALUE_FUNCTION_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_FUNCTION_HPP

#include <unicode/unistr.h>
#include <memory>

namespace dhc {
    namespace kernel {
        namespace expression {

            class Expression;

            namespace value {

                /**
                 * Represents a Haskell function
                 */
                struct Function
                {
                    Function(icu::UnicodeString a, const Expression &e);
                    Function(const Function &other);

                    Function &operator=(const Function &other);

                    icu::UnicodeString argument;
                    std::unique_ptr<Expression> exp;
                };

            }
        }
    }
}

#endif
