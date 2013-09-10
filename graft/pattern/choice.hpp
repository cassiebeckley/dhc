#ifndef DHC_LEXER_PATTERN_CHOICE_HPP
#define DHC_LEXER_PATTERN_CHOICE_HPP

#include "../pattern.hpp"

#include <initializer_list>
#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            class choice : public pattern
            {
                public:
                    choice(std::vector<std::shared_ptr<pattern>>&& p, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : pattern(type, call), pat(p) {}
                    choice(std::vector<std::shared_ptr<pattern>>&& p, int type) : pattern(type), pat(p) {}
                    choice(std::vector<std::shared_ptr<pattern>>&& p) : pattern(), pat(p) {}
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
