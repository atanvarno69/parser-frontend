#include "state.h"

namespace enjoy {

    state::state(
        std::initializer_list<
            std::pair<const unsigned, std::pair<matcher, result>>
        > il
    ) noexcept:
        matchers(il)
    {}

    unsigned state::next(
        const codepoint& input,
        state::result& result
    ) noexcept {
        for (const auto& package: matchers) {
            if (package.second.first(input)) {
                result = package.second.second;
                return package.first;
            }
        }
        result = fail;
        return 0;
    }
}