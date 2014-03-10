#ifndef DHC_GRAFT_MATCH_CHARACTER_HPP
#define DHC_GRAFT_MATCH_CHARACTER_HPP

#include "../match.hpp"
#include <unicode/utypes.h>

namespace dhc {
    namespace graft {
        namespace match {

            /**
             * \brief Represents a single matched character.
             */
            class character : public match {
                public:
                    /**
                     * \brief Creates a character match.
                     * @param column The column of the source the match
                     *               begins on.
                     * @param type An integer representing the token type.
                     * @param data The matched character.
                     */
                    character(unsigned int column, int type, UChar32 data) : match(column, type), data(data) {}

                    virtual unsigned int length();
                    virtual icu::UnicodeString flatten();
                    virtual std::vector<std::shared_ptr<match>> children();

                    /**
                     * \brief The matched character.
                     */
                    const UChar32 data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
