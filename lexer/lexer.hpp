#ifndef LEXER_HPP
#define LEXER_HPP

#include "pattern.hpp"
#include "scanner.hpp"
#include "match.hpp"

#include <memory>

namespace dhc {
    namespace lexer {
        typedef std::shared_ptr<dhc::lexer::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<dhc::lexer::match::match> match_ptr;

        class lexer {
            public:
                lexer(std::string source) : s(source) {}
                match_ptr next();
                bool tokens_left();
            private:
                scanner s;
        };

    }
}

#endif
