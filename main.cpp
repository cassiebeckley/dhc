#include "parser/parser.hpp"
#include <iostream>
#include <fstream>

#include <sstream>

#include <unicode/unistr.h>

void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        std::cout << ' ';
    }
    std::cout << "* ";
}

void print_tree(std::shared_ptr<dhc::graft::match::match> &root, int indent)
{
    if (root->type == static_cast<int>(dhc::lexer::type::WHITESPACE))
        return;

    auto tree = root->children();
    print_indent(indent);
    std::string flat;
    root->flatten().toUTF8String(flat);
    std::cout << '"' << flat << '"' << std::endl;
    for (auto it = tree.begin(); it != tree.end(); ++it)
    {
        std::vector<std::shared_ptr<dhc::graft::match::match>> c = (*it)->children();
        if (c.size() == 0)
        {
            print_indent(indent + 4);
            std::string flat;
            (*it)->flatten().toUTF8String(flat);

            size_t pos = 0;
            while ((pos = flat.find("\n", pos)) != std::string::npos)
            {
                flat.replace(pos, 1, "\\n");
                pos += 2;
            }

            std::cout << '"' << flat << '\"' << std::endl;
        }
        else
        {
            print_tree((*it), indent + 4);
        }
    }
}

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

        dhc::parser::parser p(source);

        if (!p.finished()) {
            dhc::lexer::match_ptr token (p.next());

            if (token) {
                print_tree(token, 0);
            } else {
                std::cerr << p.error(filename) << std::endl;
            }
        }
    }

    return 0;
}

