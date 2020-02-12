#ifndef PARSER_FRONTEND_AUTOMATON_H
#define PARSER_FRONTEND_AUTOMATON_H

#include <unordered_map>
#include <utility>

#include "state.h"

namespace enjoy {
    class automaton {
      private:
        unsigned current;
        std::unordered_map<unsigned, state> states;
      public:
        automaton(void) noexcept;
        automaton(
            std::initializer_list<std::pair<const unsigned, state>>
        ) noexcept;
        automaton(const automaton&) = default;
        automaton(automaton&&) noexcept = default;
        ~automaton(void) = default;
        automaton& operator=(const automaton&) = default;
        automaton& operator=(automaton&&) noexcept = default;
        state::result accept(const codepoint&) noexcept;
    };
}

#endif