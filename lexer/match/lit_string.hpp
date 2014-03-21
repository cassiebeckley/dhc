#ifndef DHC_LEXER_LIT_STRING_HPP
#define DHC_LEXER_LIT_STRING_HPP

#include "literal.hpp"
#include "lit_char.hpp"

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched lit_string.
         */
        class lit_string : public literal {
            public:
                /**
                 * \brief Creates a lit_string match.
                 * @param type An int representing the token type.
                 * @param data The matched lit_string
                 */
                lit_string (int type, std::vector<std::shared_ptr<lit_char>> data) : literal(type, literal_type::STRING), data(data) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief The matched lit_string.
                 */
                const std::vector<std::shared_ptr<lit_char>> data;
            protected:
            private:
        };

    }
}

#endif
