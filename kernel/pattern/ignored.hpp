#ifndef DHC_KERNEL_PATTERN_IGNORED_HPP
#define DHC_KERNEL_PATTERN_IGNORED_HPP

#include "../pattern.hpp"

namespace dhc {
    namespace kernel {
        namespace pattern {

            /**
             * Represents a Haskell pattern that always matched and is ignored
             * Represented by _
             */
            class Ignored : public Pattern {
                public:
                    virtual MaybeEnv test(expression::expression_ptr e);
                    virtual std::set<icu::UnicodeString> matches();

                    virtual icu::UnicodeString str();

                protected:
                private:
            };

        }
    }
}

#endif
