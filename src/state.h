#ifndef PARSER_FRONTEND_STATE_H
#define PARSER_FRONTEND_STATE_H

#include <map>
#include <utility>

#include "codepoint.h"
#include "matcher.h"

namespace enjoy {
    class state {
      public:
        enum result {
            fail,
            cont,
            accept,
            contacc,
        };
      private:
        std::map<unsigned, std::pair<matcher, result>> matchers;
      public:
        state(void) = default;
        state(
            std::initializer_list<std::pair<
                const unsigned, std::pair<matcher, result>>
            >
        ) noexcept;
        state(const state&) = default;
        state(state&&) noexcept = default;
        ~state(void) = default;
        state& operator=(const state&) = default;
        state& operator=(state&&) noexcept = default;
        unsigned next(const codepoint&, result&) noexcept;
    };
}

#endif