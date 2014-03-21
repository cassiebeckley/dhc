#ifndef DHC_LEXER_LIT_CHAR_HPP
#define DHC_LEXER_LIT_CHAR_HPP

#include "literal.hpp"

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched char.
         */
        class lit_char : public literal {
            public:
                /**
                 * \brief Creates a char match.
                 * @param type An char representing the token type.
                 * @param data The matched char (string since escape
                 *             sequences can be multiple characters)
                 */
                lit_char (int type, icu::UnicodeString data) : literal(type, literal_type::CHAR), data(data) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief The matched char.
                 */
                const icu::UnicodeString data;
            protected:
            private:
        };

    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
