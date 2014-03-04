#ifndef DHC_GRAFT_SCANNER_HPP
#define DHC_GRAFT_SCANNER_HPP

#include "match/character.hpp"

#include <memory>
#include <unicode/unistr.h>

// TODO: shouldn't have this in lib
#include <iostream>

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
                    UErrorCode e = U_ZERO_ERROR;
                    src.toUTF32(source, length, e);

                    if (U_FAILURE(e))
                    {
                        std::cerr << "scnnaer::scanner error: " << u_errorName(e) << std::endl;
                    }
                }

                virtual ~scanner();
                std::shared_ptr<match::character> next();
                scanstate get_state();
                void set_state(scanstate& state);
                bool finished();

                void newline();

                unsigned int lineno();
                unsigned int charno();
            protected:
                unsigned int length;
                UChar32 *source;

                scanstate state;
            private:
        };

    }
}

#endif // DHC_GRAFT_SCANNER_HPP
