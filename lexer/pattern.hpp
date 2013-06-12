#ifndef LEXER_PATTERN_HPP
#define LEXER_PATTERN_HPP

#include "match.hpp"
#include "scanner.hpp"

#include <memory>

namespace dhc {
    namespace lexer {
        namespace pattern {

            class pattern
            {
                public:
                    pattern(int t, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : type(t), callback(call) {}
                    pattern(int t) : pattern(t, [](std::shared_ptr<match::match> m) {return m;}) {}
                    pattern() : pattern(-1) {}

                    virtual std::shared_ptr<match::match> find(scanner& s) = 0;
                    virtual std::string str() const = 0;

                    int type;
                protected:
                    std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> callback;
            };

            std::ostream& operator<<(std::ostream& strm, const pattern& pat);

        }
    }
}

#endif // LEXER_PATTERN_HPP
