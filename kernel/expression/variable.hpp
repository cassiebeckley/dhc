#ifndef DHC_KERNEL_EXPRESSION_VARIABLE_HPP
#define DHC_KERNEL_EXPRESSION_VARIABLE_HPP

#include "../expression.hpp"

namespace dhc {
    namespace kernel {
        namespace expression {

            /**
             * Represents a Haskell variable
             */
            class Variable : public Expression {
                public:
                    /**
                     * \brief Create a variable with the name n
                     *
                     * @param n the name of the variable
                     */
                    Variable(icu::UnicodeString n) : name(n) {}

                    virtual value_ref evaluate() const;
                    virtual void bind(std::map<icu::UnicodeString, expression_ptr>) const;
                    virtual type::Type type() const;
                    virtual icu::UnicodeString str() const;

                protected:
                private:
                    icu::UnicodeString name;
                    mutable std::map<icu::UnicodeString, expression_ptr> environment;
            };

        }
    }
}

#endif
