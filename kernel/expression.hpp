#ifndef DHC_KERNEL_EXPRESSION_HPP
#define DHC_KERNEL_EXPRESSION_HPP

#include <map>
#include <memory>
#include <unicode/unistr.h>

#include "expression/application.hpp"
#include "expression/case.hpp"

namespace dhc {
    namespace kernel {

        namespace type {
            class Type;
        }

        namespace expression {

            namespace value {
                class Value;
            }
            
            class Case;

            class Expression
            {

                public:
                    Expression(Application a) : application(a), expression_type(APPLICATION) {}
                    Expression(Case c) : caseexp(c), expression_type(CASE) {}
                    Expression(value::Value v);
                    Expression(icu::UnicodeString v) : variable(std::make_pair(v, nullptr)), expression_type(VARIABLE) {}

                    Expression(const Expression &other);


                    // watever
                    Expression() : variable(std::make_pair("wat", nullptr)), expression_type(VARIABLE) {}

                    ~Expression() {}

                    value::Value evaluate() const;
                    type::Value type() const;
                    icu::UnicodeString str() const;
                    void bind(icu::UnicodeString var, Expression val);

                private:
                    union
                    {
                        Application application;
                        Case caseexp;
                        std::unique_ptr<value::Value> value;
                        std::pair<icu::UnicodeString, std::unique_ptr<Expression>> variable;
                    };

                    enum
                    {
                        APPLICATION,
                        CASE,
                        VALUE,
                        VARIABLE
                    } expression_type;
            };
        }
    }
}

#endif
