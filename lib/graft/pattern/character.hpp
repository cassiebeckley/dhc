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
                     */
                    character(UChar32 p, int type) : pattern(type), pat(p) {}

                    /**
                     * \brief Create a character pattern without a type.
                     * @param p the character to match.
                     */ 
                    character(UChar32 p) : pattern(), pat(p) {}

                    virtual std::shared_ptr<match::match> find(scanner& s);
                protected:
                private:
                    UChar32 pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
