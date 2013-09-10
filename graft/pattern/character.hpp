#ifndef DHC_LEXER_PATTERN_CHARACTER_HPP
#define DHC_LEXER_PATTERN_CHARACTER_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            class character : public pattern
            {
                public:
                    character(char p, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : pattern(type, call), pat(p) {}
                    character(char p, int type) : pattern(type), pat(p) {}
                    character(char p) : pattern(), pat(p) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    char pat;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_CHARACTER_HPP
