#ifndef DHC_GRAFT_SCANNER_HPP
#define DHC_GRAFT_SCANNER_HPP

#include "match/character.hpp"

#include <memory>
#include <unicode/unistr.h>

// TODO: shouldn't have this in lib
#include <iostream>

namespace dhc {
    namespace graft {

        /**
         * \brief A struct to hold a scanner's state.
         * @todo Either make all members private for full encapsulation
         *       and make this a friend of scanner, or make
         *       scanner::get_state() private.
         */
        struct scanstate {
            /**
             * \brief The current line number
             */
            unsigned int line_number;

            /**
             * \brief The current column number
             */
            unsigned int column;

            /**
             * \brief The current index
             */
            unsigned int index;
        };

        /**
         * \brief scanner provides an interface to keep track of
         *        the state of a source string
         */

        class scanner {
            public:
                /**
                 * \brief Create a scanner.
                 * @param src A string containing the source code.
                 */
                scanner(icu::UnicodeString src) : state({0, 0, 0})
                {
                    // separate function because Doxygen would get confused
                    initialize(src);
                }

                /**
                 * \brief Free memory allocated by the scanner.
                 */
                virtual ~scanner();

                /**
                 * \brief Returns the next character in the source string.
                 *
                 * Also modifies the scanner state. Match objects
                 * must restore the state if they do not find a match.
                 *
                 * @return a match::character containing the next character.
                 */
                std::shared_ptr<match::character> next();

                /**
                 * \brief Returns the current state of the scanner.
                 *
                 * Use this to back up the scanner's state.
                 * @todo Consider simply changing state to be public.
                 *
                 * @return The current state.
                 */
                scanstate get_state();

                /**
                 * \brief Sets the current state of the scanner.
                 *
                 * Use this to restore the scanner to an earlier state.
                 *
                 * @param state the state to restore.
                 */
                void set_state(scanstate& state);

                /**
                 * \brief Check if the scanner is finished.
                 * @return Whether or not the scanner has reached the
                 *         end of the source.
                 */
                bool finished();

                /**
                 * \brief Increment the line number.
                 */
                void newline();

                /**
                 * @return The line number.
                 */
                unsigned int lineno();

                /**
                 * @return The character number.
                 */
                unsigned int charno();
            protected:
            private:
                unsigned int length;
                UChar32 *source;

                scanstate state;
                void initialize(icu::UnicodeString);
       };

    }
}

#endif // DHC_GRAFT_SCANNER_HPP
