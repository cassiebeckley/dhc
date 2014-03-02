#ifndef DHC_GRAFT_SCANNER_HPP
#define DHC_GRAFT_SCANNER_HPP

#include "match/character.hpp"

#include <memory>
#include <string>
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {

        struct scanstate {
            unsigned int line_number;
            unsigned int column;
            unsigned int index;
        };

        class scanner {
            public:
                scanner(icu::UnicodeString src) : state({0, 0, 0})
                {
                    length = src.countChar32();
                    source = new UChar32[length];
                    UErrorCode e;
                    src.toUTF32(source, length, e);
                }

                virtual ~scanner();

                /* TODO: bad bad bad
                 *       fix this so it returns a more specific match object
                 *       ideally for the atomic type of the scanner
                 */
                std::shared_ptr<match::match> next();
                scanstate get_state();
                void set_state(scanstate& state);
                bool finished();

                void newline();

                unsigned int lineno();
                unsigned int charno();
            protected:
                int length;
                UChar32 *source;

                scanstate state;
            private:
        };

    }
}

#endif // DHC_GRAFT_SCANNER_HPP
