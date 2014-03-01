#ifndef DHC_GRAFT_PATTERN_STRING_HPP
#define DHC_GRAFT_PATTERN_STRING_HPP

#include "compound.hpp"
#include "character.hpp"
#include "../match/string.hpp"

#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace pattern {

            class string : public compound {
                public:
                    string(icu::UnicodeString pat, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : compound(std::vector<std::shared_ptr<pattern>>(), type, call), str_pat(pat)
                    {
                        initialize();
                    }
                    string(icu::UnicodeString pat, int type) : compound(std::vector<std::shared_ptr<pattern>>(), type), str_pat(pat)
                    {
                        initialize();
                    }
                    string(icu::UnicodeString pat) : string(pat, -1) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;
                protected:
                    icu::UnicodeString str_pat;
                    void initialize();
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_STRING_HPP
