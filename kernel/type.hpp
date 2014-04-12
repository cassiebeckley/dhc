#ifndef DHC_KERNEL_TYPE_HPP
#define DHC_KERNEL_TYPE_HPP

#include <unicode/unistr.h> 
#include <vector>

namespace dhc {
    namespace kernel {
        namespace type {
            
            /**
             * Represents a Haskell type
             */
            class Type {
                public:
                    Type(std::vector<icu::UnicodeString> cs) : constructors(cs) {}
                    /**
                     * \brief Evaluate the type.
                     *
                     * @return the resulting type value
                     */
                    //virtual TypeValue evaluate() = 0;

                    const std::vector<icu::UnicodeString> constructors;

                protected:
                private:
            };

        }
    }
}

#endif
