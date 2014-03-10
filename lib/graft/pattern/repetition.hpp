#ifndef DHC_GRAFT_PATTERN_REPETITION_HPP
#define DHC_GRAFT_PATTERN_REPETITION_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a pattern repeated
             *        an arbitrary number of times.
             */
            class repetition : public pattern {
                public:
                    /**
                     * \brief Create a repetition pattern.
                     * @param pat The repeated pattern.
                     * @param type The type of the matched token.
                     * @param callback a callback that performs additional
                     *                 processing on the match returned
                     */
                    repetition(std::shared_ptr<pattern> pat, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(pat) {}

                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner& s);

                private:
                    std::shared_ptr<pattern> pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_REPETITION_HPP
