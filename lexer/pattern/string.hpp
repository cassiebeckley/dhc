#ifndef DHC_LEXER_PATTERN_STRING_HPP
#define DHC_LEXER_PATTERN_STRING_HPP

#include "compound.hpp"
#include "character.hpp"
#include "../match/string.hpp"

namespace dhc {
    namespace lexer {
        namespace pattern {

            class string : public compound {
                public:
                    string(std::string pat) : compound(std::vector<std::shared_ptr<pattern>>()), str_match(new match::string(pat))
                    {
                        for (auto it = pat.begin(); it != pat.end(); ++it) {
                            std::shared_ptr<character> pat (new character(*it));
                            this->pat.push_back(pat);
                        }
                    }
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::shared_ptr<match::string> str_match;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_STRING_HPP
