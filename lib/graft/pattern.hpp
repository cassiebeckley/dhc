#ifndef DHC_GRAFT_PATTERN_HPP
#define DHC_GRAFT_PATTERN_HPP

#include "match.hpp"
#include "scanner.hpp"

#include <memory>

namespace dhc {
    namespace graft {
        namespace pattern {

            typedef std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> match_func;

            /**
             * \brief Represents a pattern to search for.
             */
            class pattern
            {
                public:
                    /**
                     * \brief Create a pattern.
                     * @param type The type of the matched token.
                     * @param callback A callback that performs additional
                     *                 processing on the match::match
                     *                 created by findmatch(). Guaranteed
                     *                 to be passed a non-null match.
                     */
                    pattern(int type = -1, match_func callback = nullptr) : type(type), process(callback) {}

                    /**
                     * \brief Test if the pattern matches the scanner.
                     *
                     * If it does, the scanner state is kept, otherwise,
                     * the scanner is reset to the previous state.
                     *
                     * This calls findmatch(), defined in a subclass,
                     * and passes the result through the callback
                     * passed to the constructor.
                     *
                     * @param s the scanner to test.
                     * @return A match, if a match is found, and nullptr if
                     *         one is not.
                     */
                    std::shared_ptr<match::match> find(scanner& s);

                protected:
                    /**
                     * \brief Test if the pattern matches the scanner.
                     *
                     * If it does, the scanner state is kept, otherwise,
                     * the scanner is reset to the previous state.
                     * 
                     * The results from this function are processed
                     * through a callback passed to the constructor.
                     *
                     * @param s the scanner to test.
                     * @return A match, if a match is found, and nullptr if
                     *         one is not.
                     */
                    virtual std::shared_ptr<match::match> findmatch(scanner& s) = 0;

                    /**
                     * \brief The type of the matched token.
                     */
                    int type;
                private:
                    match_func process;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_HPP
