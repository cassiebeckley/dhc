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
                     */
                    repetition(std::shared_ptr<pattern> pat, int type) : pattern(type), pat(pat) {}

                    /**
                     * \brief Create a repetition pattern without a type.
                     * @param pat The repeated pattern.
                     */
                    repetition(std::shared_ptr<pattern> pat) : pattern(), pat(pat) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);

                protected:
                private:
                    std::shared_ptr<pattern> pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_REPETITION_HPP
