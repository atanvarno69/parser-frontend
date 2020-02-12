#ifndef PARSER_FRONTEND_MATCHERFACTORY_H
#define PARSER_FRONTEND_MATCHERFACTORY_H

#include <utility>

#include "matcher.h"

namespace enjoy {
    class matcherFactory {
      public:
        matcherFactory(void) = delete;
        matcherFactory(const matcherFactory&) = delete;
        matcherFactory(matcherFactory&&) = delete;
        ~matcherFactory(void) = delete;
        matcherFactory& operator=(const matcherFactory&) = delete;
        matcherFactory& operator=(matcherFactory&&) = delete;
        static matcher is(const codepoint) noexcept;
        static matcher range(const codepoint, const codepoint) noexcept;
        static matcher neg(const matcher&) noexcept;
        static matcher conjunction(std::initializer_list<matcher>) noexcept;
        static matcher alternate(std::initializer_list<codepoint>) noexcept;
        static matcher alternate(std::initializer_list<matcher>) noexcept;
    };
}

#endif