#ifndef DHC_KERNEL_PATTERN_HPP
#define DHC_KERNEL_PATTERN_HPP

#include "maybe.hpp"
#include <unicode/unistr.h>
#include <map>

#include "pattern/constructor.hpp"

namespace dhc {
    namespace kernel {
        namespace expression {
            class Expression;
        }

        namespace pattern {

            typedef dhc::kernel::Maybe<std::map<icu::UnicodeString, expression::Expression>> MaybeEnv;

            /**
             * Represents a Haskell pattern
             */
            class Pattern
            {
                public:
                    Pattern(Constructor c) : constructor(c), pattern_type(CONSTRUCTOR) {}
                    Pattern(icu::UnicodeString v) : variable(v), pattern_type(VARIABLE) {}
                    Pattern() : pattern_type(IGNORED) {}

                    Pattern(const Pattern &other);

                    ~Pattern() {}
                    /**
                     * \brief Test if an expression matches the pattern
                     *
                     * @param e The expression to test
                     * @return A Maybe, which contains a std::map mapping variables to their bound expressions on success
                     */
                    MaybeEnv test(const expression::Expression &e) const;

                    icu::UnicodeString str() const;
                private:
                    union
                    {
                        Constructor constructor;
                        icu::UnicodeString variable;
                    };

                    enum
                    {
                        CONSTRUCTOR,
                        VARIABLE,
                        IGNORED
                    } pattern_type;
            };
        }
    }
}

#endif
