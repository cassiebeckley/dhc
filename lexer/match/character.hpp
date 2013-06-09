#ifndef DHC_LEXER_MATCH_CHARACTER_HPP
#define DHC_LEXER_MATCH_CHARACTER_HPP

#include "../match.hpp"

namespace dhc {
    namespace lexer {
        namespace match {

            class character : public match {
                public:
                    character(unsigned int column, int type, char d) : match(column, type), data(d) {}
                    virtual unsigned int length();
                    virtual std::string flatten();

                    const char data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_CHARACTER_HPP
