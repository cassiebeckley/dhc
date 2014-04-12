#ifndef DHC_KERNEL_EXPRESSION_HPP
#define DHC_KERNEL_EXPRESSION_HPP

#include <map>
#include <unicode/unistr.h>
#include <memory>

#include "type.hpp"

namespace dhc {
    namespace kernel {

        namespace expression {

            namespace value {
                class Value;
            }

            class Expression;

            typedef std::shared_ptr<Expression> expression_ptr;
            typedef std::shared_ptr<value::Value> value_ptr;

            /**
             * Represents a Haskell expression
             */
            class Expression {
                public:
                    /**
                     * \brief Destroy the expression.
                     */
                    virtual ~Expression() {}

                    /**
                     * \brief Evaluate the expression.
                     *
                     * @return the resulting value
                     */
                    virtual value::Value &evaluate() = 0;

                    /**
                     * \brief Substitute values for variables.
                     *
                     * @param env The variables to substitute in
                     * @return the resulting expression
                     */
                    virtual expression_ptr bind(std::map<icu::UnicodeString, expression_ptr> env) = 0;

                    /**
                     * \brief Return the Hindley-Milner type of the expression.
                     *
                     * @return the type of the expression
                     */
                    virtual type::Type type() = 0;


                    virtual icu::UnicodeString str() = 0;

                protected:
                private:
            };

        }
    }
}

#endif
