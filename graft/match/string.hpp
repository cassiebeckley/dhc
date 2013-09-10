#ifndef DHC_LEXER_MATCH_STRING_HPP
#define DHC_LEXER_MATCH_STRING_HPP

#include "../match.hpp"

#include <string>

namespace dhc {
    namespace graft {
        namespace match {

            class string : public match {
                public:
                    string(unsigned int column, int type, std::string d) : match(column, type), data(d) {}
                    virtual unsigned int length();
                    virtual std::string flatten();

                    const std::string data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_STRING_HPP
