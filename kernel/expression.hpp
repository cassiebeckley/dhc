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

            typedef std::shared_ptr<const Expression> expression_ptr;
            typedef const value::Value& value_ref;

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
                    virtual value_ref evaluate() const = 0;

                    /**
                     * \brief Bind an environment to the expression.
                     *
                     * @param env The environment.
                     */
                    virtual void bind(std::map<icu::UnicodeString, expression_ptr> env) const = 0;

                    /**
                     * \brief Return the Hindley-Milner type of the expression.
                     *
                     * @return the type of the expression
                     */
                    virtual type::Type type() const = 0;


                    virtual icu::UnicodeString str() const = 0;

                protected:
                private:
                    std::map<icu::UnicodeString, expression_ptr> env;
            };

        }
    }
}

#endif
