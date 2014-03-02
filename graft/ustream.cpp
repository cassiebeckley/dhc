#include "ustream.hpp"

std::ostream &operator<<(std::ostream &out, const icu::UnicodeString &str)
{
    std::string utf8;
    str.toUTF8String(utf8);
    return out << utf8;
}
