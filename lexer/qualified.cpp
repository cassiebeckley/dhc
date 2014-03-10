#include "qualified.hpp"

#include <sstream>
#include <graft/match/string.hpp>

unsigned int dhc::lexer::qualified::length()
{
    int length = 0; 

    for (auto it = modules.begin(); it != modules.end(); ++it)
    {
        if (it != modules.begin())
            length++;

        length += it->countChar32();
    }

    return length;
}

icu::UnicodeString dhc::lexer::qualified::flatten()
{
    icu::UnicodeString result;

    for (auto it = modules.begin(); it != modules.end(); ++it)
    {
        if (it != modules.begin())
            result.append('.');

        result.append(*it);
    }

    return result;
}

std::vector<std::shared_ptr<dhc::graft::match::match>> dhc::lexer::qualified::children()
{
    std::vector<std::shared_ptr<dhc::graft::match::match>> children;

    for (auto it = modules.begin(); it != modules.end(); ++it)
    {
        children.push_back(std::make_shared<graft::match::string>(column, type, *it));
    }

    return children;
}
