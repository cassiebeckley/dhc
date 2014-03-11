#ifndef DHC_GRAFT_SCANNER_CHARACTER_HPP
#define DHC_GRAFT_SCANNER_CHARACTER_HPP

#include "../scanner.hpp"

#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace scanner {

            /**
             * \brief scanner provides an interface to keep track of
             *        the state of a source string
             * @todo update docs here
             */
            class character : public scanner {
                public:
                    /**
                     * \brief Create a scanner.
                     * @param src A string containing the source code.
                     */
                    character(icu::UnicodeString src);

                    /**
                     * \brief Free memory allocated by the scanner.
                     */
                    virtual ~character();

                    /**
                     * \brief Returns the next character in the source string.
                     *
                     * Also modifies the scanner state. Match objects
                     * must restore the state if they do not find a match.
                     *
                     * @return a match::character containing the next character.
                     */
                    virtual std::shared_ptr<match::match> next();

                    /**
                     * \brief Check if the scanner is finished.
                     * @return Whether or not the scanner has reached the
                     *         end of the source.
                     */
                    virtual bool finished();

                protected:
                private:
                    unsigned int length;
                    UChar32 *source;
           };
        }
    }
}

#endif
