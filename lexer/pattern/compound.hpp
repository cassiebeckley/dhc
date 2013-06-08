#ifndef DHC_LEXER_PATTERN_COMPOUND_HPP
#define DHC_LEXER_PATTERN_COMPOUND_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace lexer {
        namespace pattern {

            class compound : public pattern
            {
                public:
                    compound(std::vector<std::shared_ptr<pattern>>&& pattern) : pat(pattern) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;

                    void add_pattern(std::shared_ptr<pattern> pat);
                protected:
                    std::vector<std::shared_ptr<pattern>> pat;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_COMPOUND_HPP
