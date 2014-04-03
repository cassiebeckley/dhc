#ifndef DHC_LEXER_LAYOUT_HPP
#define DHC_LEXER_LAYOUT_HPP

#include "lexer.hpp"

#include <stack>
#include <queue>

#include <unicode/unistr.h>


// TODO: remove
#include <iostream>

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
                layout(icu::UnicodeString source) : lex(source), max_index(0)
                {
                    match_ptr m;
                    while ((m = generate_next()) != nullptr)
                    {
                        final_tokens.push_back(m);
                    }
                }

                virtual match_ptr next();
                virtual bool finished();

                /**
                 * \brief Move '}' to just before the point where
                 *        parsing failed.
                 *
                 * @return the index where a '}' was inserted
                 */
                unsigned int reset();

            private:
                match_ptr generate_next();

                lexer lex;
                unsigned int max_index;
                std::stack<int> context;
                std::queue<int> angle_tokens;
                std::queue<match_ptr> tokens;

                std::vector<match_ptr> final_tokens;
        };

    }
}

#endif
