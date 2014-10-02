#ifndef DHC_KERNEL_EXPRESSION_VALUE_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_HPP

#include "../expression.hpp"

#include "value/function.hpp"
#include "value/uservalue.hpp"

#include "../type.hpp"

namespace dhc {
    namespace kernel {
        namespace expression {
            namespace value {

                class Value
                {
                    public:
                        Value (Function f) : function(f), value_type(FUNCTION) {}
                        Value (UserValue u) : uservalue(u), value_type(USERVALUE) {}

                        Value (const Value &other);

                        ~Value() {}

                        Value &operator=(const Value &other);

                        /**
                         * \brief Return the constructor number of the value
                         *
                         * @return the constructor
                         */
                        unsigned int constructor() const;

                        /**
                         * \brief Access the constructor fields
                         *
                         * @param i the field index
                         * @return the expression at index i
                         */
                        const Expression &at(unsigned int i) const;

                        type::Value type() const;

                        icu::UnicodeString str() const;
                    private:
                        friend class dhc::kernel::expression::Expression;
                        union
                        {
                            Function function;
                            UserValue uservalue;
                        };

                        enum
                        {
                            FUNCTION,
                            USERVALUE
                        } value_type;

                        void bind(icu::UnicodeString var, Expression val);
                };

            }
        }
    }
}
#endif
