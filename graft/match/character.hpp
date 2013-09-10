#ifndef DHC_GRAFT_MATCH_CHARACTER_HPP
#define DHC_GRAFT_MATCH_CHARACTER_HPP

#include "../match.hpp"

namespace dhc {
    namespace graft {
        namespace match {

            class character : public match {
                public:
                    character(unsigned int column, int type, char d) : match(column, type), data(d) {}
                    virtual unsigned int length();
                    virtual std::string flatten();

                    const char data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
