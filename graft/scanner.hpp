#ifndef SCANNER_HPP
#define SCANNER_HPP

#include "match/character.hpp"

#include <memory>
#include <string>

namespace dhc {
    namespace graft {

        struct scanstate {
            unsigned int line_number;
            unsigned int column;
            unsigned int index;
        };

        class scanner {
            public:
                scanner(std::string source) : source(source), state({0, 0, 0})
                {
                    this->source += "\n";
                }
                std::shared_ptr<match::match> lookahead();
                void consume();
                scanstate get_state();
                void set_state(scanstate& state);
                bool finished();

                void newline();

                unsigned int lineno();
                unsigned int charno();
            protected:
                std::string source;

                scanstate state;
            private:
        };

    }
}

#endif // SCANNER_HPP
