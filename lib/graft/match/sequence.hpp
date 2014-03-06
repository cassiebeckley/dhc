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
                     * @param column The column of the source the match
                     *               begins on.
                     * @param type An integer representing the token type.
                     * @param data The matched sequence.
                     */
                    sequence(unsigned int column, int type, std::vector<std::shared_ptr<match>> data) : match(column, type), data(data) {}
                    virtual int unsigned length();
                    virtual icu::UnicodeString flatten();

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
