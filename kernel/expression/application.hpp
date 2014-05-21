#ifndef DHC_KERNEL_EXPRESSION_APPLICATION_HPP
#define DHC_KERNEL_EXPRESSION_APPLICATION_HPP

#include "../expression.hpp"
#include <memory>

namespace dhc {
    namespace kernel {
        namespace expression {

            /**
             * Represents a Haskell function application
             */
            class Application : public Expression {
                public:
                    /**
                     * \brief Create a function application
                     *
                     * @param f The function to call
                     * @param f The argument to apply
                     */
                    Application(expression_ptr f, expression_ptr a) : function(f), argument(a) {}
                    virtual value_ref evaluate() const;
                    virtual void bind(std::map<icu::UnicodeString, expression_ptr>) const;
                    virtual type::Type type() const;
                    virtual icu::UnicodeString str() const;

                protected:
                private:
                    // Must have a function type
                    expression_ptr function;

                    expression_ptr argument;
            };

        }
    }
}

#endif
