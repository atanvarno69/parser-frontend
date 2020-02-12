#include "automaton.h"

namespace enjoy {

    automaton::automaton(void) noexcept: current(0), states() {}

    automaton::automaton(
        std::initializer_list<std::pair<const unsigned, state>> il
    ) noexcept: current(0), states(il)
    {}

    state::result automaton::accept(const codepoint& input) noexcept {
        state::result output = state::fail;
        current = states.at(current).next(input, output);
        return output;
    }
}