#ifndef DHC_GRAFT_USTREAM_HPP
#define DHC_GRAFT_USTREAM_HPP

#include <iostream>
#include <unicode/unistr.h>

std::ostream &operator<<(std::ostream &out, const icu::UnicodeString &str);

#endif
