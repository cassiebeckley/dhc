#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Hello world, you usage-ignoring meanie!" << std::endl;
        return -1;
    }

    std::ifstream file (argv[1], std::ios::in | std::ios::binary | std::ios::ate);
    std::string source;

    if (!file.is_open()) {
        std::cerr << "I can't open that file. I hate you too." << std::endl;
        return -1;
    }

    source.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    source.assign((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());

    file.close();

    dhc::lexer::lexer lex(source);

    while (!lex.finished()) {
        dhc::lexer::match_ptr token (lex.next());

        if (token) {
            std::cout << "\"" << token->flatten() << "\"" << std::endl;
        } else {
            std::cout << lex.error(argv[1]) << std::endl;
            break;
        }
    }

    return 0;
}
/*
int main(int argc, char** argv)
{
    while (true) {
        std::cout << "> ";
        char buffer[512];
        std::cin.getline(buffer, 512);
        std::string source(buffer);

        dhc::lexer lex(source);

        std::shared_ptr<dhc::lexer_match> token;

        while (token = lex.next()) {
            if (token) {
                std::cout << "\"" << token->flatten() << "\"" << std::endl;
            }
        }

        std::cout << std::endl << "Done" << std::endl;
    }

    return 0;
}
*/
