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
                    string(std::string pat, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : compound(std::vector<std::shared_ptr<pattern>>(), type, call), str_pat(pat)
                    {
                        initialize();
                    }
                    string(std::string pat, int type) : compound(std::vector<std::shared_ptr<pattern>>(), type), str_pat(pat)
                    {
                        initialize();
                    }
                    string(std::string pat) : string(pat, -1) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::string str_pat;
                    void initialize();
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_STRING_HPP
