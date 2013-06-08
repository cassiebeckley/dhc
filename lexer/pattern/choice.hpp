#ifndef DHC_LEXER_PATTERN_CHOICE_HPP
#define DHC_LEXER_PATTERN_CHOICE_HPP

#include "../pattern.hpp"

#include <initializer_list>
#include <memory>
#include <vector>

namespace dhc {
    namespace lexer {
        namespace pattern {

            class choice : public pattern
            {
                public:
                    choice(std::vector<std::shared_ptr<pattern>>&& pattern) : pat(pattern) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual std::string str() const;
                protected:
                    std::vector<std::shared_ptr<pattern>> pat;
                private:
            };

        }
    }
}

#endif // DHC_LEXER_PATTERN_CHOICE_HPP
