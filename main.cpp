#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

#include <unicode/unistr.h>

int main(int argc, char** argv)
{
    while (true)
    {
        UErrorCode e = U_ZERO_ERROR;
        std::string filename = argc > 1 ? argv[1] : "main";

        std::cout << "> ";
        char utf8[512];
        std::cin.getline(utf8, 512);

        UChar utf16[512];
        u_strFromUTF8(utf16, sizeof(utf16), NULL, utf8, -1, &e);
        if (U_FAILURE(e))
        {
            std::cerr << "error converting to UTF16: " << u_errorName(e) << std::endl;
        }

        icu::UnicodeString source(utf16);
        source.append("\n");

        dhc::lexer::lexer lex(source);

        while (!lex.finished()) {
            dhc::lexer::match_ptr token (lex.next());

            if (token) {
                std::string flat;
                token->flatten().toUTF8String(flat);
                std::cout << "\"" << flat << "\": " << lex.typenames[token->type] << std::endl;
            } else {
                std::cerr << lex.error(filename) << std::endl;
                return -1;
            }
        }
    }

    return 0;
}

