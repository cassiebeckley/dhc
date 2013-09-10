#include "pattern.hpp"

std::ostream& dhc::graft::pattern::operator<<(std::ostream& strm, const dhc::graft::pattern::pattern& pat)
{
    return strm << pat.str();
}
