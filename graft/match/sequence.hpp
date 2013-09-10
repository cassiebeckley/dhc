#ifndef DHC_LEXER_MATCH_SEQUENCE_HPP
#define DHC_LEXER_MATCH_SEQUENCE_HPP

#include "../match.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace match {

            class sequence : public match {
                public:
                    sequence(unsigned int column, int type, std::vector<std::shared_ptr<match>> d) : match(column, type), data(d) {}
                    virtual int unsigned length();
                    virtual std::string flatten();

                    const std::vector<std::shared_ptr<match>> data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_LEXER_MATCH_SEQUENCE_HPP
