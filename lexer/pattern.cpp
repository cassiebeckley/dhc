#include "pattern.hpp"

std::ostream& dhc::lexer::pattern::operator<<(std::ostream& strm, const dhc::lexer::pattern::pattern& pat)
{
    return strm << pat.str();
}
