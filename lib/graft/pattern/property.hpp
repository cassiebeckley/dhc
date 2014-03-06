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
                     */
                    property(icu::UnicodeString str, int type) : pattern(type)
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

                    /**
                     * @brief Create a property pattern without a type.
                     * @param str A string which will be used to create
                     *            an icu::UnicodeSet. See ICU's
                     *            documentation for further details.
                     */
                    property(icu::UnicodeString str) : property(str, -1) {}

                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                private:
                    icu::UnicodeSet pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
