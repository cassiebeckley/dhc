#ifndef DHC_KERNEL_TYPE_HPP
#define DHC_KERNEL_TYPE_HPP

#include <unicode/unistr.h> 
#include <vector>
#include "typeerror.hpp"
#include "type/value.hpp"

namespace dhc {
    namespace kernel {
        namespace type {
            
            /**
             * Represents a Haskell type
             */
            class Type {
                public:
                    Type(Value v) : value(v), type_type(VALUE) {}
                    Type(const Type &other)
                    {
                        switch (other.type_type)
                        {
                            case VALUE:
                                this->type_type = VALUE;
                                this->value = value;
                                break;
                        }
                    }

                    ~Type() {}

                    /**
                     * \brief Evaluate the type.
                     *
                     * @return the resulting type value
                     */
                    //virtual TypeValue evaluate() = 0;

                    icu::UnicodeString str() const;

                protected:
                private:
                    union
                    {
                        Value value;
                    };

                    enum
                    {
                        VALUE
                    } type_type;
            };

        }
    }
}

#endif
