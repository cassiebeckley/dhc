#ifndef DHC_KERNEL_EXPRESSION_VALUE_USERVALUE_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_USERVALUE_HPP

#include "../value.hpp"
#include <vector>

namespace dhc {
    namespace kernel {
        namespace expression {
            namespace value {

                /**
                 * Represents a Haskell value of a user-defined type
                 */
                class UserValue : public Value {
                    public:
                        /**
                         * \brief Create a value
                         *
                         * @param c The data constructor used
                         * @param f The values of the fields
                         * @param t The type of the value
                         */
                        UserValue(unsigned int c, std::vector<expression_ptr> f, type::Type t) : ctr(c), fields(f), datatype(t) {}
                        virtual Value &evaluate();
                        virtual expression_ptr bind(std::map<icu::UnicodeString, expression_ptr>);
                        virtual type::Type type();
                        virtual unsigned int constructor();
                        virtual dhc::kernel::expression::expression_ptr at(int i);

                        virtual icu::UnicodeString str();

                    protected:
                    private:
                        unsigned int ctr;
                        std::vector<expression_ptr> fields;
                        type::Type datatype;
                };

            }
        }
    }
}

#endif
