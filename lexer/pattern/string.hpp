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
                    string(std::string pat, int type) : compound(std::vector<std::shared_ptr<pattern>>(), type), str_pat(pat)
                    {
                        for (auto it = pat.begin(); it != pat.end(); ++it) {
                            std::shared_ptr<character> pat (new character(*it, -1));
                            this->pat.push_back(pat);
                        }
                    }
                    string(std::string pat) : string(pat, -1) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::string str_pat;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_STRING_HPP
