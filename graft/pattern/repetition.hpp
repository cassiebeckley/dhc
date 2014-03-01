#ifndef DHC_GRAFT_PATTERN_REPETITION_HPP
#define DHC_GRAFT_PATTERN_REPETITION_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            class repetition : public pattern {
                public:
                    repetition(std::shared_ptr<pattern> pat, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : pattern(type, call), pat(pat) {}
                    repetition(std::shared_ptr<pattern> pat, int type) : pattern(type), pat(pat) {}
                    repetition(std::shared_ptr<pattern> pat) : pattern(), pat(pat) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;
                protected:
                    std::shared_ptr<pattern> pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_REPETITION_HPP
