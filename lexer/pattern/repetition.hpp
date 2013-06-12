#ifndef DHC_LEXER_PATTERN_REPETITION_HPP
#define DHC_LEXER_PATTERN_REPETITION_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace lexer {
        namespace pattern {

            class repetition : public pattern {
                public:
                    repetition(std::shared_ptr<pattern> pat, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : pattern(type, call), pat(pat) {}
                    repetition(std::shared_ptr<pattern> pat, int type) : pattern(type), pat(pat) {}
                    repetition(std::shared_ptr<pattern> pat) : pattern(), pat(pat) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::shared_ptr<pattern> pat;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_REPETITION_HPP
