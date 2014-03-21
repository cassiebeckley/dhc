#ifndef DHC_LEXER_DIGIT_HPP
#define DHC_LEXER_DIGIT_HPP

#include <graft/match.hpp>

namespace dhc {
    namespace lexer {

        /**
         * \brief Represents a matched digit.
         */
        class digit : public graft::match::match {
            public:
                /**
                 * \brief Creates a digit match.
                 * @param type An integer representing the token type.
                 * @param data The matched character.
                 * @param d The matched digit.
                 */
                digit (int type, UChar32 data, int d) : match(type), data(data), d(d) {}

                virtual unsigned int length();
                virtual icu::UnicodeString flatten();
                virtual std::vector<std::shared_ptr<match>> children();

                /**
                 * \brief The matched character.
                 */
                const UChar32 data;

                /**
                 * \brief The matched digit.
                 */
                const int d;

            protected:
            private:
        };

    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
