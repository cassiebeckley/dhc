#ifndef DHC_LEXER_MATCH_SEQUENCE_HPP
#define DHC_LEXER_MATCH_SEQUENCE_HPP

#include "../match.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace lexer {
        namespace match {

            class sequence : public match {
                public:
                    sequence(std::vector<std::shared_ptr<match>> d) : data(d) {}
                    virtual int length();
                    virtual std::string flatten();

                    const std::vector<std::shared_ptr<match>> data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_SEQUENCE_HPP
