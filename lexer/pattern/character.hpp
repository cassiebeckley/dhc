#ifndef DHC_LEXER_PATTERN_CHARACTER_HPP
#define DHC_LEXER_PATTERN_CHARACTER_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

namespace dhc {
    namespace lexer {
        namespace pattern {

            class character : public pattern
            {
                public:
                    character(char pattern) : char_match(new match::character(pattern)) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::shared_ptr<match::character> char_match;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_CHARACTER_HPP
