#ifndef DHC_GRAFT_PATTERN_EXCLUDE_HPP
#define DHC_GRAFT_PATTERN_EXCLUDE_HPP

#include "../pattern.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            class exclude : public pattern {
                public:
                    exclude(std::shared_ptr<pattern> pat, std::shared_ptr<pattern> exc, int type) : pattern(type), pat(pat), exc(exc) {}
                    exclude(std::shared_ptr<pattern> pat, std::shared_ptr<pattern> exc) : pattern(), pat(pat), exc(exc) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;
                protected:
                    std::shared_ptr<pattern> pat;
                    std::shared_ptr<pattern> exc;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_EXCLUDE_HPP
