#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

int main(int argc, char** argv)
{
    while (true) {
        std::cout << "> ";
        char buffer[512];
        std::cin.getline(buffer, 512);
        std::string source(buffer);

        dhc::lexer::lexer lex(source);

        while (!lex.finished()) {
            dhc::lexer::match_ptr token (lex.next());

            if (token) {
                std::string flat;
                token->flatten().toUTF8String(flat);
                std::cout << "\"" << flat << "\": " << lex.typenames[token->type] << std::endl;
            } else {
                std::cerr << lex.error(argv[1]) << std::endl;
                return -1;
            }
        }
    }

    return 0;
}

