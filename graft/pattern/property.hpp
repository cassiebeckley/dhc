#ifndef DHC_GRAFT_PATTERN_PROPERTY_HPP
#define DHC_GRAFT_PATTERN_PROPERTY_HPP

#include "../pattern.hpp"
#include "../match/character.hpp"

#include <unicode/unistr.h>
#include <unicode/uniset.h>

// TODO: remove
#include <iostream>

namespace dhc {
    namespace graft {
        namespace pattern {

            class property : public pattern
            {
                public:
                    property(icu::UnicodeString str, int type) : pattern(type)
                    {
                        UErrorCode e = U_ZERO_ERROR;
                        pat = UnicodeSet(str, e);
                        if (U_FAILURE(e))
                        {
                            // ignore, perhaps? :|
                            // an exception might be nice
                            std::cerr << "Error: " << u_errorName(e) << ", couldn't create Unicode set" << std::endl;
                        }
                    }

                    property(icu::UnicodeString pat) : property(pat, -1) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;
                protected:
                    icu::UnicodeSet pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_CHARACTER_HPP
