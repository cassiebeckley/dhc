#ifndef DHC_GRAFT_SCANNER_CHARACTER_HPP
#define DHC_GRAFT_SCANNER_CHARACTER_HPP

#include "../scanner.hpp"

#include <unicode/unistr.h>

namespace dhc {
    namespace graft {
        namespace scanner {

            /**
             * \brief scanner provides an interface to keep track of
             *        the state of a source string
             * @todo update docs here
             */
            class character : public scanner {
                public:
                    /**
                     * \brief Create a scanner.
                     * @param src A string containing the source code.
                     */
                    character(icu::UnicodeString src);

                    virtual ~character();
                    virtual std::shared_ptr<match::match> next();
                    virtual bool finished();

                protected:
                private:
                    unsigned int length;
                    UChar32 *source;
           };
        }
    }
}

#endif
