#ifndef DHC_LEXER_LIT_FLOAT_HPP
#define DHC_LEXER_LIT_FLOAT_HPP

#include "literal.hpp"

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched lit_float.
         */
        class lit_float : public literal {
            public:
                /**
                 * \brief Creates a lit_float match.
                 * @param type An lit_float representing the token type.
                 * @param length The original matched length.
                 * @param integral The integer part of the float.
                 * @param fractional The fractional part of the float.
                 * @param exponent The exponential part of the float.
                 */
                lit_float (int type, int length, int64_t integral, int64_t fractional, int64_t exponent) : literal(type, literal_type::FLOAT), len(length), integral(integral), fractional(fractional), exponent(exponent) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief the original matched length
                 */
                const int len;

                /**
                 * \brief (12).34e5
                 */
                const int64_t integral;

                /**
                 * \brief 12.(34)e5
                 */
                const int64_t fractional;

                /**
                 * \brief 12.34e(5)
                 */
                const int64_t exponent;
            protected:
            private:
        };

    }
}

#endif
