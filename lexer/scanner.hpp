#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <map>
#include <set>
#include <string>
#include <stack>

namespace dhc {

    class scanner {
        public:
            scanner(std::string source) : source(source), index(0)
            {
                this->source += "\n";
            }
            char peek();
            char get();
            unsigned int get_index();
            void set_index(int i);
            bool finished();

            void newline();

            unsigned int lineno();
            unsigned int charno();
        protected:
            std::string source;
            unsigned int index;
            unsigned int line_number;

            std::map<unsigned int, std::set<unsigned int>> line_number_to_index;
            std::map<unsigned int, unsigned int> index_to_line_number;
        private:
    };

}

#endif // SCANNER_HPP
