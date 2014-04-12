#ifndef DHC_KERNEL_PATTERN_CONSTRUCTOR_HPP
#define DHC_KERNEL_PATTERN_CONSTRUCTOR_HPP

#include "../pattern.hpp"

namespace dhc {
    namespace kernel {
        namespace pattern {

            /**
             * Represents a Haskell constructor pattern
             */
            class Constructor : public Pattern {
                public:
                    /**
                     * \brief Create a constructor pattern
                     *
                     * @param c the constructor
                     * @param p the fields of the constructor
                     * @param t the type of the constructor
                     */
                    Constructor(unsigned int c, std::vector<std::shared_ptr<dhc::kernel::pattern::Pattern>> p, type::Type t) : constructor(c), fields(p), datatype(t) {}

                    virtual MaybeEnv test(std::shared_ptr<expression::Expression> e);
                    virtual icu::UnicodeString str();

                protected:
                private:
                    unsigned int constructor;
                    std::vector<std::shared_ptr<dhc::kernel::pattern::Pattern>> fields;

                    type::Type datatype;
            };

        }
    }
}

#endif
