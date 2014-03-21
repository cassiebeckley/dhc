#ifndef DHC_GRAFT_MATCH_HPP
#define DHC_GRAFT_MATCH_HPP

#include <string>
#include <vector>
#include <memory>
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
                     * @param type An integer representing the token type.
                     */
                    match(int type) : type(type) {}
                    /**
                     * \brief Returns the length of the match.
                     * @return The number of characters matched.
                     */
                    virtual unsigned int length() = 0;

                    /**
                     * \brief Represent the match as a string.
                     *
                     * Returns the matched characters as a string.
                     * @return A string containing the matched characters.
                     */
                    virtual icu::UnicodeString flatten() = 0;

                    /**
                     * \brief Return this node's children.
                     *
                     * @return A vector containing this node's children.
                     */
                    virtual std::vector<std::shared_ptr<match>> children() = 0;

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
