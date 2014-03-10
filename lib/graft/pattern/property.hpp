#ifndef DHC_GRAFT_PATTERN_PROPERTY_HPP
#define DHC_GRAFT_PATTERN_PROPERTY_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

#include <unicode/unistr.h>
#include <unicode/uniset.h>

// TODO: remove
#include <iostream>

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a character based on
             *        an icu::UnicodeSet
             */
            class property : public pattern
            {
                public:
                    /**
                     * @brief Create a property pattern.
                     * @param str A string which will be used to create
                     *            an icu::UnicodeSet. See ICU's
                     *            documentation for further details.
                     * @param type The type of the matched token.
                     * @param callback a callback that performs additional
                     *                 processing on the match returned
                     */
                    property(icu::UnicodeString str, int type = -1, match_func callback = nullptr) : pattern(type, callback)
                    {
                        UErrorCode e = U_ZERO_ERROR;
                        pat = UnicodeSet(str, e);
                        if (U_FAILURE(e))
                        {
                            // ignore, perhaps? :|
                            // an exception might be nice
                            std::cerr << "Error: " << u_errorName(e) << ", couldn't create Unicode set" << std::endl;
                        }
                    }

                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner::scanner& s);

                private:
                    icu::UnicodeSet pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
