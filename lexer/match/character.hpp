#ifndef DHC_LEXER_MATCH_CHARACTER_HPP
#define DHC_LEXER_MATCH_CHARACTER_HPP

#include "../match.hpp"

namespace dhc {
    namespace lexer {
        namespace match {

            class character : public match {
                public:
                    character(char d) : data(d) {}
                    virtual int length();
                    virtual std::string flatten();

                    const char data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_CHARACTER_HPP
