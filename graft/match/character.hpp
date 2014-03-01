#ifndef DHC_GRAFT_MATCH_CHARACTER_HPP
#define DHC_GRAFT_MATCH_CHARACTER_HPP

#include "../match.hpp"
#include <unicode/utypes.h>

namespace dhc {
    namespace graft {
        namespace match {

            class character : public match {
                public:
                    character(unsigned int column, int type, UChar32 d) : match(column, type), data(d) {}
                    virtual unsigned int length();
                    virtual icu::UnicodeString flatten();

                    const UChar32 data;
                protected:
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_MATCH_CHARACTER_HPP
