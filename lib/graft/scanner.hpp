#ifndef DHC_GRAFT_SCANNER_HPP
#define DHC_GRAFT_SCANNER_HPP

#include "match/character.hpp"

#include <memory>
#include <unicode/unistr.h>

// TODO: shouldn't have this in lib
#include <iostream>

namespace dhc {
    namespace graft {
        class scanner;

        /**
         * \brief A struct to hold a scanner's state.
         */
        class scanstate {
            private:
                friend scanner;
                scanstate() : line_number(0), column(0), index(0) {}
                unsigned int line_number;
                unsigned int column;
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
                scanner(icu::UnicodeString src);

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
                 * @return The column number.
                 */
                unsigned int charno();

                /**
                 * @return The index.
                 */
                unsigned int index();

                /**
                 * \brief The current state of the scanner.
                 */
                scanstate state;
            protected:
            private:
                unsigned int length;
                UChar32 *source;

                void initialize(icu::UnicodeString);
       };

    }
}

#endif // DHC_GRAFT_SCANNER_HPP
