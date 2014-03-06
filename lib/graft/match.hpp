#ifndef DHC_GRAFT_MATCH_HPP
#define DHC_GRAFT_MATCH_HPP

#include <string>
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace match {

            /**
             * \brief Represents the result of matching a pattern.
             */
            class match {
                public:
                    /**
                     * \brief Creates a match.
                     * @param column The column of the source the match
                     *               begins on.
                     * @param type An integer representing the token type.
                     */
                    match(unsigned int column, int type) : column(column), type(type) {}
                    /**
                     * \brief Returns the length of the match.
                     * @return The number of characters matched.
                     */
                    virtual unsigned int length() = 0;

                    /**
                     * \brief Represent the match as a tree
                     *
                     * Don't use this, removing soon
                     * @param indent the number of characters to indent
                     * @deprecated Seriously, don't use it.
                     */
                    virtual icu::UnicodeString tree(unsigned int indent) = 0;

                    /**
                     * \brief Represent the match as a string.
                     *
                     * Returns the matched characters as a string.
                     * @return A string containing the matched characters.
                     */
                    virtual icu::UnicodeString flatten() = 0;

                    /**
                     * \brief The column in the source where the match begins
                     */
                    const unsigned int column;

                    /**
                     * \brief The token type of the match.
                     */
                    int type;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_HPP
