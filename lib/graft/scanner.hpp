#ifndef DHC_GRAFT_SCANNER_SCANNER_HPP
#define DHC_GRAFT_SCANNER_SCANNER_HPP

#include "match.hpp"
#include <memory>

namespace dhc {
    namespace graft {
        namespace scanner {

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
                     * \brief Free memory allocated by the scanner.
                     */
                    virtual ~scanner() {}

                    /**
                     * \brief Returns the next character in the source string.
                     *
                     * Also modifies the scanner state. Match objects
                     * must restore the state if they do not find a match.
                     *
                     * @return a match::character containing the next character.
                     */
                    virtual std::shared_ptr<match::match> next()=0;

                    /**
                     * \brief Check if the scanner is finished.
                     * @return Whether or not the scanner has reached the
                     *         end of the source.
                     */
                    virtual bool finished()=0;

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

                    /**
                     * Allow child classes to modify the line number
                     * @return current line number
                     */
                    unsigned int &state_line_number();

                    /**
                     * Allow child classes to modify the column number
                     * @return current column number
                     */
                    unsigned int &state_column();

                    /**
                     * Allow child classes to modify the index
                     * @return current index
                     */
                    unsigned int &state_index();
                private:
           };
        }
    }
}

#endif
