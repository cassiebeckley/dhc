#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>
#include "graft/ustream.hpp"

int main(int argc, char** argv)
{
    while (true) {
        std::cout << "> ";
        char buffer[512];
        std::cin.getline(buffer, 512);
        std::string utf8(buffer);

        icu::UnicodeString source;
        source.fromUTF8(utf8);

        dhc::lexer::lexer lex(source);

        while (!lex.finished()) {
            dhc::lexer::match_ptr token (lex.next());

            if (token) {
                std::cout << "\"" << token->flatten() << "\": " << lex.typenames[token->type] << std::endl;
            } else {
                std::cerr << lex.error(argv[1]) << std::endl;
                return -1;
            }
        }
    }

    return 0;
}

