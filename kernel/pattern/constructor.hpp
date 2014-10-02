#ifndef DHC_KERNEL_PATTERN_CONSTRUCTOR_HPP
#define DHC_KERNEL_PATTERN_CONSTRUCTOR_HPP

#include "../type.hpp"

namespace dhc {
    namespace kernel {
        namespace pattern {
            class Pattern;

            struct Constructor
            {
                Constructor(unsigned int ctr, std::vector<Pattern> fields, type::Type datatype) : ctr(ctr), fields(fields), datatype(datatype) {}
                unsigned int ctr;
                std::vector<Pattern> fields;
                type::Type datatype;
            };

        }
    }
}

#endif
