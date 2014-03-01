#ifndef DHC_GRAFT_PATTERN_COMPOUND_HPP
#define DHC_GRAFT_PATTERN_COMPOUND_HPP

#include "../pattern.hpp"

#include "../match/sequence.hpp"

#include <memory>
#include <vector>

namespace dhc {
    namespace graft {
        namespace pattern {

            class compound : public pattern
            {
                public:
                    compound(std::vector<std::shared_ptr<pattern>>&& p, int type, std::function<std::shared_ptr<match::match>(std::shared_ptr<match::match>)> call) : pattern(type, call), pat(p) {}
                    compound(std::vector<std::shared_ptr<pattern>>&& p, int type) : pattern(type), pat(p) {}
                    compound(std::vector<std::shared_ptr<pattern>>&& p) : pattern(), pat(p) {}
                    virtual std::shared_ptr<match::match> find(scanner& s);
                    virtual icu::UnicodeString str() const;

                    void add_pattern(std::shared_ptr<pattern> pat);
                protected:
                    std::vector<std::shared_ptr<pattern>> pat;
                private:
            };

        }
    }
}

#endif // DHC_GRAFT_PATTERN_COMPOUND_HPP
