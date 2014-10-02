#ifndef DHC_KERNEL_TYPEERROR
#define DHC_KERNEL_TYPEERROR

#include <exception>

namespace dhc
{
    namespace kernel
    {
        class TypeError : public std::exception
        {
            public:
                TypeError(std::string s) : msg(s) {}
            private:
                std::string msg;
        };
    }
}

#endif
