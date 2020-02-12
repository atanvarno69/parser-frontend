#include "automatonFactory.h"
#include "matcherFactory.h"

namespace enjoy {

    automaton* automatonFactory::singleSymbol(const codepoint& input) {
        matcher m = matcherFactory::is(input);
        return automatonFactory::singleSymbol(m);
    }

    automaton* automatonFactory::singleSymbol(const matcher& input) {
        state s {{0, {input, state::accept}}};
        return new automaton {{0, s}};
    }
}