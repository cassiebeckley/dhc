#ifndef DHC_GRAFT_PATTERN_STRING_HPP
#define DHC_GRAFT_PATTERN_STRING_HPP

#include "compound.hpp"
#include "character.hpp"
#include "../match/string.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            /**
             * \brief A pattern that matches a string of characters.
             *
             * @todo Support Unicode.
             */
            class string : public compound {
                public:
                    /**
                     * \brief Create a string pattern.
                     * @param str the string to match.
                     * @param type the matched token's type
                     */
                    string(std::string str, int type) : compound(std::vector<std::shared_ptr<pattern>>(), type), str_pat("")
                    {
                        for (auto it = str.begin(); it != str.end(); ++it) {
                            std::shared_ptr<character> p (new character(*it, -1));
                            this->pat.push_back(p);
                            this->str_pat.append(*it);
                        }

                    }

                    /**
                     * \brief Create a string pattern without a type.
                     * @param str the string to match.
                     */
                    string(std::string str) : string(str, -1) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                private:
                    icu::UnicodeString str_pat;
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_STRING_HPP
