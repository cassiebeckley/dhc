#ifndef DHC_GRAFT_PATTERN_EXCLUDE_HPP
#define DHC_GRAFT_PATTERN_EXCLUDE_HPP

#include "../pattern.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a pattern as long as
             *        another pattern doesn't match.
             */
            class exclude : public pattern {
                public:
                    /**
                     * \brief Create an exclusion pattern.
                     * @param pat the pattern to match.
                     * @param exc the pattern to exclude.
                     * @param type the type of the matched token.
                     * @param callback a callback that performs additional
                     *                 processing on the match returned
                     */
                    exclude(std::shared_ptr<pattern> pat, std::shared_ptr<pattern> exc, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(pat), exc(exc) {}

                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner& s);

                private:
                    std::shared_ptr<pattern> pat;
                    std::shared_ptr<pattern> exc;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_EXCLUDE_HPP
