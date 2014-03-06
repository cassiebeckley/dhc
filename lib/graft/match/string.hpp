#ifndef DHC_GRAFT_MATCH_STRING_HPP
#define DHC_GRAFT_MATCH_STRING_HPP

#include "../match.hpp"

#include <string>
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace match {

            /**
             * \brief Represents a matched string.
             */
            class string : public match {
                public:
                    /**
                     * \brief Creates a string match.
                     * @param column The column of the source the match
                     *               begins on.
                     * @param type An integer representing the token type.
                     * @param data The matched string.
                     */
                    string(unsigned int column, int type, icu::UnicodeString data) : match(column, type), data(data) {}
                    virtual unsigned int length();
                    virtual icu::UnicodeString flatten();

                    /**
                     * \brief The matched string.
                     */
                    const icu::UnicodeString data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_STRING_HPP
