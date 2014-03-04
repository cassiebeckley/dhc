#ifndef DHC_GRAFT_PATTERN_CHARACTER_HPP
#define DHC_GRAFT_PATTERN_CHARACTER_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

namespace dhc {
    namespace graft {
        namespace pattern {

            class character : public pattern
            {
                public:
                    character(char p, int type) : pattern(type), pat(p) {}
                    character(char p) : pattern(), pat(p) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;
                protected:
                    UChar32 pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
