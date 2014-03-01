#include "pattern.hpp"

std::ostream& dhc::graft::pattern::operator<<(std::ostream& strm, const dhc::graft::pattern::pattern& pat)
{
    std::string result;
    pat.str().toUTF8String(result);

    return strm << result;
}
