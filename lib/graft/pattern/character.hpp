#ifndef DHC_GRAFT_PATTERN_CHARACTER_HPP
#define DHC_GRAFT_PATTERN_CHARACTER_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a single character.
             *
             * This might be replaced by the pattern::property class
             * over time, which is based on icu::UnicodeSet and thus
             * is more powerful.
             */
            class character : public pattern
            {
                public:
                    /**
                     * \brief Create a character pattern.
                     * @param p the character to match.
                     * @param type the type of the pattern.
                     * @param callback a callback to perform additional
                     *                 processing on the match returned
                     */
                    character(UChar32 p, int type = -1, match_func callback = nullptr) : pattern(type, callback), pat(p) {}

                protected:
                    virtual std::shared_ptr<match::match> findmatch(scanner::scanner& s);

                private:
                    UChar32 pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
