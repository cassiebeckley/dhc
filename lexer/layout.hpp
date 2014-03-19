#ifndef DHC_LEXER_LAYOUT_HPP
#define DHC_LEXER_LAYOUT_HPP

#include "lexer.hpp"

#include <stack>
#include <queue>

#include <unicode/unistr.h>

namespace dhc {
    namespace lexer {
        typedef std::shared_ptr<graft::pattern::pattern> pattern_ptr;
        typedef std::shared_ptr<graft::match::match> match_ptr;

        /**
         * \brief Applies the layout rules for Haskell 2010.
         *
         * Like, stuff
         */
        class layout : public graft::scanner::scanner {
            public:
                /**
                 * \brief Initialize
                 * 
                 * @param source the source code to analyze
                 */
                layout(icu::UnicodeString source) : lex(source) {}

                virtual match_ptr next();
                virtual bool finished();
                virtual graft::scanner::scanstate &state();

            private:
                lexer lex;
                std::stack<int> context;
                std::queue<int> angle_tokens;
                std::queue<match_ptr> tokens;
        };

    }
}

#endif
