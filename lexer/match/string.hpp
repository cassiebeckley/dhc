#ifndef DHC_LEXER_MATCH_STRING_HPP
#define DHC_LEXER_MATCH_STRING_HPP

#include "../match.hpp"

#include <string>

namespace dhc {
    namespace lexer {
        namespace match {

            class string : public match {
                public:
                    string(std::string d) : data(d) {}
                    virtual int length();
                    virtual std::string flatten();

                    const std::string data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_STRING_HPP
