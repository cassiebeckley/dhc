#ifndef DHC_KERNEL_TYPE_VALUE_HPP
#define DHC_KERNEL_TYPE_VALUE_HPP

#include <unicode/unistr.h> 
#include <vector>

namespace dhc {
    namespace kernel {
        namespace type {
            
            struct Value {
                Value(std::vector<icu::UnicodeString> &&ctrs) : constructors(ctrs) {}
                Value() : constructors() {}
                std::vector<icu::UnicodeString> constructors;
            };

        }
    }
}

#endif
