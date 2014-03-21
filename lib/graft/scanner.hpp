#ifndef DHC_GRAFT_SCANNER_SCANNER_HPP
#define DHC_GRAFT_SCANNER_SCANNER_HPP

#include "match.hpp"
#include <memory>

namespace dhc {
    namespace graft {
        namespace scanner {

            /**
             * \brief scanner provides an interface to keep track of
             *        the state of a source string
             */
            class scanner {
                public:
                    /**
                     * Initialize the scanner
                     */
                    scanner() : index(0) {}

                    /**
                     * \brief Free memory allocated by the scanner.
                     */
                    virtual ~scanner() {}

                    /**
                     * \brief Returns the next character in the source string.
                     *
                     * Also modifies the scanner state. Pattern objects
                     * must restore the state if they do not find a match.
                     *
                     * @return a match_ptr containing the match
                     */
                    virtual std::shared_ptr<match::match> next()=0;

                    /**
                     * \brief Check if the scanner is finished.
                     * @return Whether or not the scanner has reached the
                     *         end of the source.
                     */
                    virtual bool finished()=0;

                    /**
                     * \brief The index.
                     */
                    unsigned int index;

                protected:
                private:
           };
        }
    }
}

#endif
