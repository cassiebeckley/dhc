#ifndef DHC_KERNEL_EXPRESSION_VALUE_USERVALUE_HPP
#define DHC_KERNEL_EXPRESSION_VALUE_USERVALUE_HPP

#include "../../type/value.hpp"

#include <vector>
#include <memory>

namespace dhc {
    namespace kernel {

        namespace expression {

            namespace value {

                /**
                 * Represents a Haskell value of a user-defined type
                 */
                struct UserValue
                {
                    UserValue(unsigned int ctr, std::vector<Expression> &fields, type::Value datatype);
                    UserValue(const UserValue &other);

                    unsigned int ctr;
                    std::vector<std::unique_ptr<Expression>> fields;
                    type::Value datatype;
                };

            }
        }
    }
}

#endif
