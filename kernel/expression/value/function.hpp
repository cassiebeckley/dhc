#ifndef DHC_KERNEL_EXPRESSION_VALUE_FUNCTION_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_FUNCTION_HPP

#include "../value.hpp"

namespace dhc {
    namespace kernel {
        namespace expression {
            namespace value {

                /**
                 * Represents a Haskell function
                 */
                class Function : public Value {
                    public:
                        /**
                         * \brief Create a function
                         *
                         * @param a A string with the argument identifier
                         * @param e The expression
                         */
                        Function(icu::UnicodeString a, expression_ptr e) : argument(a), exp(e) {}
                        virtual value_ref evaluate() const;
                        virtual void bind(std::map<icu::UnicodeString, expression_ptr>) const;
                        virtual type::Type type() const;
                        virtual unsigned int constructor() const;
                        virtual dhc::kernel::expression::expression_ptr at(int i) const;

                        virtual icu::UnicodeString str() const;

                        /**
                         * \brief Apply the function
                         *
                         * @param e the expression the apply the function to
                         * @return the resulting expression
                         */
                        expression_ptr apply(expression_ptr e);

                    protected:
                    private:
                        icu::UnicodeString argument;
                        expression_ptr exp;
                };

            }
        }
    }
}

#endif
