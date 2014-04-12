#ifndef DHC_KERNEL_EXPRESSION_VALUE_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_HPP

#include "../expression.hpp"

namespace dhc {
    namespace kernel {
        namespace expression {
            namespace value {

                /**
                 * An abstract representation of a Haskell value
                 */
                class Value : public Expression {
                    public:
                        /**
                         * \brief Return the constructor number of the value
                         *
                         * @return the constructor
                         */
                        virtual unsigned int constructor() = 0;

                        /**
                         * \brief Access the constructor fields
                         *
                         * @param i the field index
                         * @return the expression at index i
                         */
                        virtual dhc::kernel::expression::expression_ptr at(int i) = 0;
                };

            }
        }
    }
}
#endif
