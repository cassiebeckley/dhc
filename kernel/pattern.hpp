#ifndef DHC_KERNEL_PATTERN_HPP
#define DHC_KERNEL_PATTERN_HPP

#include "expression.hpp"
#include "expression/value.hpp"
#include "maybe.hpp"
#include <unicode/unistr.h>
#include <memory>

namespace dhc {
    namespace kernel {
        namespace pattern {

            typedef dhc::kernel::Maybe<std::map<icu::UnicodeString, dhc::kernel::expression::expression_ptr>> MaybeEnv;

            /**
             * Represents a Haskell pattern
             */
            class Pattern {
                public:
                    /**
                     * \brief Test if an expression matches the pattern
                     *
                     * @param e The expression to test
                     * @return A Maybe, which contains a std::map mapping variables to their bound expressions on success
                     */
                    virtual MaybeEnv test(expression::expression_ptr e) = 0;
                    virtual icu::UnicodeString str() = 0;

                protected:
                private:
            };
        }
    }
}

#endif
