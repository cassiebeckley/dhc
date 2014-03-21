#ifndef DHC_GRAFT_PATTERN_TYPE_HPP
#define DHC_GRAFT_PATTERN_TYPE_HPP

#include "../pattern.hpp"
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches matches of a specific type.
             */
            class type : public pattern
            {
                public:
                    /**
                     * \brief Create a type pattern.
                     * @param pat the match type to match.
                     * @param str an optional string which must match.
                     * @param type the type of the pattern.
                     * @param callback a callback to perform additional
                     *                 processing on the match returned
                     */
                    type(int pat, icu::UnicodeString str, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(pat), str(str) {}

                    /**
                     * \brief Create a type pattern.
                     * @param pat the match type to match.
                     * @param type the type of the pattern.
                     * @param callback a callback to perform additional
                     *                 processing on the match returned
                     */
                    type(int pat, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(pat), str("") {}

                protected:
                    virtual std::shared_ptr<graft::match::match> findmatch(scanner::scanner& s);

                private:
                    int pat;
                    icu::UnicodeString str;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
