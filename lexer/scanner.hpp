#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <string>
#include <stack>

namespace dhc {

    class scanner {
        public:
            scanner(std::string source) : source(source), index(0) {}
            char peek();
            char get();
            int get_index();
            void set_index(int i);
        protected:
            std::string source;
            int index;
        private:
    };

}

#endif // SCANNER_HPP
