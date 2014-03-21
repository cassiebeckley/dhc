#ifndef DHC_GRAFT_MATCH_SEQUENCE_HPP
#define DHC_GRAFT_MATCH_SEQUENCE_HPP

#include "../match.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace match {

            /**
             * \brief Represents a sequence of matches.
             */
            class sequence : public match {
                public:
                    /**
                     * \brief Creates a sequence match.
                     * @param type An integer representing the token type.
                     * @param data The matched sequence.
                     */
                    sequence(int type, std::vector<std::shared_ptr<match>> data) : match(type), data(data) {}
                    virtual int unsigned length();
                    virtual icu::UnicodeString flatten();
                    virtual std::vector<std::shared_ptr<match>> children();


                    /**
                     * \brief The matched sequence.
                     */
                    const std::vector<std::shared_ptr<match>> data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_SEQUENCE_HPP
