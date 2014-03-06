#ifndef DHC_GRAFT_PATTERN_HPP
#define DHC_GRAFT_PATTERN_HPP

#include "match.hpp"
#include "scanner.hpp"

#include <memory>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief Represents a pattern to search for.
             */
            class pattern
            {
                public:
                    /**
                     * \brief Create a pattern.
                     * @param type The type of the matched token.
                     */
                    pattern(int type) : type(type) {}

                    /**
                     * \brief Create a pattern with no type.
                     */
                    pattern() : pattern(-1) {}

                    /**
                     * \brief Test if the pattern matches the scanner.
                     *
                     * If it does, the scanner state is kept, otherwise,
                     * the scanner is reset to the previous state.
                     * @param s the scanner to test.
                     * @return A match, if a match is found, and nullptr if
                     *         one is not.
                     */
                    virtual std::shared_ptr<match::match> find(scanner& s) = 0;

                    /**
                     * \brief Returns a string representation of the pattern
                     * @deprecated Really bad idea
                     *
                     * Recursion and whatnot.
                     */
                    virtual std::string str() const = 0;

                protected:
                    /**
                     * \brief The type of the matched token.
                     */
                    int type;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_HPP
