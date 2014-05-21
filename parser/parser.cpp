#include "parser.hpp"
#include <sstream>

std::shared_ptr<dhc::graft::match::match> dhc::parser::parser::parse()
{
    if (lay.finished())
    {
        return nullptr;
    }

    std::shared_ptr<graft::match::match> m;
    unsigned int prev_index = 0;
    unsigned int max_index = 0;
    while ((m = final->find(lay)) == nullptr && (max_index = lay.reset()) > prev_index)
    {
        prev_index = max_index;
    }

    return m;
}

bool dhc::parser::parser::finished()
{
    return lay.finished();
}

std::string dhc::parser::parser::error(std::string filename)
{
    std::stringstream ss;
    ss << filename << std::endl;
    return ss.str();
}
