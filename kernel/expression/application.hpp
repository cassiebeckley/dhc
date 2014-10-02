#ifndef DHC_KERNEL_EXPRESSION_APPLICATION_HPP
#define DHC_KERNEL_EXPRESSION_APPLICATION_HPP

#include <memory>

namespace dhc {
    namespace kernel {
        namespace expression {
            class Expression;

            /**
             * Represents a Haskell function application
             */
            struct Application
            {
                Application(const Expression &f, const Expression &a);
                Application(const Application &other) : Application(*(other.function), *(other.argument)) {}

                Application &operator=(const Application &other);

                std::unique_ptr<Expression> function;
                std::unique_ptr<Expression> argument;
            };

        }
    }
}

#endif
