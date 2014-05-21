#ifndef DHC_KERNEL_EXPRESSION_CASE_HPP
#define DHC_KERNEL_EXPRESSION_CASE_HPP

#include "../expression.hpp"
#include "../pattern.hpp"

#include <memory>
#include <vector>

#include <cstdlib>
#include <unicode/ustream.h>

namespace dhc {
    namespace kernel {
        namespace expression {
            
            typedef std::shared_ptr<pattern::Pattern> pattern_ptr;

            /**
             * Represents a Haskell case expression
             */
            class Case : public Expression {
                public:
                    /**
                     * \brief Create a case expression
                     *
                     * @param p vector of patterns to choose from
                     */
                    Case(expression_ptr e, std::vector<std::pair<pattern_ptr, expression_ptr>> p) : exp(e), patterns(p) {}

                    virtual value_ref evaluate() const;
                    virtual void bind(std::map<icu::UnicodeString, expression_ptr>) const;
                    virtual type::Type type() const;

                    virtual icu::UnicodeString str() const;

                protected:
                private:
                    expression_ptr exp;
                    std::vector<std::pair<pattern_ptr, expression_ptr>> patterns;
            };
        }
    }
}

#endif
