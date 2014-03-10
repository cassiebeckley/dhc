#ifndef DHC_GRAFT_PATTERN_CHOICE_HPP
#define DHC_GRAFT_PATTERN_CHOICE_HPP

#include "../pattern.hpp"

#include <initializer_list>
#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches one of multiple patterns.
             *
             * The longest match is used. For single-character matches,
             * pattern::property should be faster.
             */
            class choice : public pattern
            {
                public:
                    /**
                     * Create a choice pattern.
                     * @param p a vector containing the patterns
                     *          to choose from.
                     * @param type the type of the pattern
                     * @param callback a callback to perform additional
                     *        processing on the match returned
                     */
                    choice(std::vector<std::shared_ptr<pattern>>&& p, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(p) {}

                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner& s);

                    /**
                     * \brief The list of patterns to choose from.
                     */
                    std::vector<std::shared_ptr<pattern>> pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHOICE_HPP
