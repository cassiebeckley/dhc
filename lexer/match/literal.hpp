#ifndef DHC_LEXER_LITERAL_HPP
#define DHC_LEXER_LITERAL_HPP

#include <graft/match.hpp>

namespace dhc {
    namespace lexer {

        enum class literal_type
        {
            INTEGER,
            FLOAT,
            CHAR,
            STRING
        };

        /**
         * \brief Abstract class that represents a matched literal.
         */
        class literal : public graft::match::match {
            public:
                /**
                 * \brief Creates a literal match.
                 * @param type An integer representing the token type.
                 * @param lit_type A literal_type representing the literal type.
                 */
                literal (int type, literal_type lit_type) : match(type), lit_type(lit_type) {}

                /**
                 * \brief The literal type.
                 */
                const literal_type lit_type;
            protected:
            private:
        };

    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
