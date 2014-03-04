#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

#include <unicode/unistr.h>
#include <unicode/ucnv.h>

void printu(UConverter *cnv, const UnicodeString &s)
{
    char out[200];
    UErrorCode e = U_ZERO_ERROR;

    out[s.extract(out, sizeof(out), cnv, e)] = 0;
    if (U_FAILURE(e))
    {
        std::cerr << "error " << u_errorName(e) << std::endl;
        return;
    }

    std::cout << out;
}

int main(int argc, char** argv)
{
    UConverter *cnv;
    UErrorCode e = U_ZERO_ERROR;

    std::string filename = argc > 1 ? argv[1] : "main";

    cnv = ucnv_open(NULL, &e);
    if (U_FAILURE(e))
    {
        std::cerr << "error " << u_errorName(e) << " opening the default converter" << std::endl;
        return e;
    }

    while (true) {
        std::cout << "> ";
        char utf8[512];
        std::cin.getline(utf8, 512);

        std::cout << "Before conversion: \"" << utf8 << '"' << std::endl;

        UChar utf16[512];
        u_strFromUTF8(utf16, sizeof(utf16), NULL, utf8, -1, &e);
        if (U_FAILURE(e))
        {
            std::cerr << "error " << u_errorName(e) << std::endl;
        }

        icu::UnicodeString source(utf16);

        std::cout << "After conversion: \"";
        printu(cnv, source);
        std::cout << '"' << std::endl;

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

    ucnv_close(cnv);

    return 0;
}

