#ifndef LEXER_MATCH_HPP
#define LEXER_MATCH_HPP

#include <string>

namespace dhc {
    namespace graft {
        namespace match {

            class match {
                public:
                    match(unsigned int c, int t) : column(c), type(t) {}
                    virtual unsigned int length() = 0;
                    virtual std::string flatten() = 0;
                    const unsigned int column;

                    int type;
                protected:
                private:
            };

        }
    }
}

#endif // LEXER_MATCH_HPP
