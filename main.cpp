#include "lexer/lexer.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

// original version from http://stackoverflow.com/a/5878802/963937
std::string myreplace(std::string &s,
                      std::string toReplace,
                      std::string replaceWith)
{
    auto pos = s.find(toReplace);
    if (pos != std::string::npos)
        return(s.replace(pos, toReplace.length(), replaceWith));
    else
        return s;
}

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

    std::stringstream ss;
    ss  << "<html>\n<head>\n<title>" << argv[1] << "</title>\n"
        << "<style type=\"text/css\">\n"
        << "body { background-color: #000000 }\n"
        << ".whitespace { color: #aaaaaa }\n"
        << ".qvarid { font-weight: bolder; color: #ffff00 }\n"
        << ".qconid { color: #00ffff }\n"
        << ".qvarsym { color: #ff00ff }\n"
        << ".qconsym { color: #ab0010 }\n"
        << ".literal { color: #ffffff }\n"
        << ".special { color: #ff0000 }\n"
        << ".reservedop { color: #00ab10 }\n"
        << ".reservedid { color: #00ff00 }\n"
        << "</style>" << std::endl << "<body>\n"
        << "<pre>" << std::endl;

    while (!lex.finished()) {
        dhc::lexer::match_ptr token (lex.next());

        if (token) {
            ss << "<span class=\"" << lex.typenames[token->type] << "\">" << token->flatten() << "</span>";
        } else {
            std::cout << lex.error(argv[1]) << std::endl;
            return -1;
        }
    }

    ss << "</pre></body>" << std::endl;

    std::cout << ss.str();

    return 0;
}
