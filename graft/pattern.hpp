#ifndef DHC_GRAFT_PATTERN_HPP
#define DHC_GRAFT_PATTERN_HPP

#include "match.hpp"
#include "scanner.hpp"

#include <memory>
#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace pattern {

            class pattern
            {
                public:
                    pattern(int t) : type(t) {}
                    pattern() : pattern(-1) {}

                    virtual std::shared_ptr<match::match> find(scanner& s) = 0;
                    virtual icu::UnicodeString str() const = 0;

                    int type;
                protected:
                    std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> callback;
            };

            std::ostream& operator<<(std::ostream& strm, const pattern& pat);

        }
    }
}

#endif // DHC_GRAFT_PATTERN_HPP
