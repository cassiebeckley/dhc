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
                     */
                    exclude(std::shared_ptr<pattern> pat, std::shared_ptr<pattern> exc, int type) : pattern(type), pat(pat), exc(exc) {}

                    /**
                     * \brief Create an exclusion pattern without a type
                     * @param pat the pattern to match.
                     * @param exc the pattern to exclude.
                     */
                    exclude(std::shared_ptr<pattern> pat, std::shared_ptr<pattern> exc) : pattern(), pat(pat), exc(exc) {}

                    virtual std::shared_ptr<match::match> find(scanner& s);

                protected:
                private:
                    std::shared_ptr<pattern> pat;
                    std::shared_ptr<pattern> exc;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_EXCLUDE_HPP
