#ifndef LEXER_MATCH_HPP
#define LEXER_MATCH_HPP

#include <string>

namespace dhc {
    namespace lexer {
        namespace match {

            class match {
                public:
                    virtual int length() = 0;
                    virtual std::string flatten() = 0;
                protected:
                private:
            };

        }
    }
}

#endif // LEXER_MATCH_HPP