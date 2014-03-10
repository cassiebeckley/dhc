#ifndef DHC_LEXER_INTEGER_HPP
#define DHC_LEXER_INTEGER_HPP

#include "literal.hpp"

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched integer.
         */
        class integer : public literal {
            public:
                /**
                 * \brief Creates a integer match.
                 * @param column The column of the source the match
                 *               begins on.
                 * @param type An integer representing the token type.
                 * @param length The original matched length.
                 * @param data The matched integer.
                 */
                integer (unsigned int column, int type, int length, int64_t data) : literal(column, type, literal_type::INTEGER), len(length), data(data) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief the original matched length
                 */
                const int len;

                /**
                 * \brief the matched integer.
                 */
                const int64_t data;
            protected:
            private:
        };

    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
