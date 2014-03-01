#ifndef DHC_GRAFT_MATCH_STRING_HPP
#define DHC_GRAFT_MATCH_STRING_HPP

#include "../match.hpp"

#include <string>
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace match {

            class string : public match {
                public:
                    string(unsigned int column, int type, UnicodeString d) : match(column, type), data(d) {}
                    virtual unsigned int length();
                    virtual UnicodeString flatten();

                    const UnicodeString data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_STRING_HPP
