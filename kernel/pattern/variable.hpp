#ifndef DHC_KERNEL_PATTERN_VARIABLE_HPP
#define DHC_KERNEL_PATTERN_VARIABLE_HPP

#include "../pattern.hpp"

namespace dhc {
    namespace kernel {
        namespace pattern {

            /**
             * Represents a Haskell variable pattern
             */
            class Variable : public Pattern {
                public:
                    /**
                     * \brief Create a variable pattern with the name n
                     *
                     * @param n the name of the variable
                     */
                    Variable(icu::UnicodeString n) : name(n) {}

                    virtual MaybeEnv test(expression::expression_ptr e);
                    virtual std::set<icu::UnicodeString> matches();

                    virtual icu::UnicodeString str();

                protected:
                private:
                    icu::UnicodeString name;
            };

        }
    }
}

#endif
