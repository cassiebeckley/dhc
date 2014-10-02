#include "ast/module.hpp"

#include <iostream>
#include <unicode/ustream.h>

int main()
{
    dhc::ast::module::module m;
    auto env = m.load();

    std::cout << """Printing environment""" << std::endl;
    for (auto it = env.begin(); it != env.end(); ++it)
    {
        env["main"].bind(it->first, it->second);
        std::cout << it->first << " = " << it->second.str() << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Type of main:" << std::endl;
    std::cout << dhc::kernel::type::Type(env["main"].type()).str() << std::endl;

    std::cout << std::endl;

    std::cout << "Evaluating main" << std::endl;

    std::cout << std::endl << "Type of main when evaluated:" << std::endl;
    std::cout << dhc::kernel::type::Type(env["main"].evaluate().type()).str() << std::endl;


    return 0;
}

