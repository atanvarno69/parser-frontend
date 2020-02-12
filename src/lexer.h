#ifndef PARSER_FRONTEND_LEXER_H
#define PARSER_FRONTEND_LEXER_H

#include <queue>
#include <unordered_map>
#include <utility>

#include "automaton.h"
#include "index.h"
#include "scanner.h"
#include "token.h"

namespace enjoy {
    struct tracker {
        bool crFlag;
        index position;
        tracker(void);
        tracker(const tracker&) = default;
        tracker(tracker&&) = default;
        ~tracker(void) = default;
        tracker& operator=(const tracker&) = default;
        tracker& operator=(tracker&&) = default;
        void incrementColumn(void) noexcept;
        void incrementLine(void) noexcept;
        void increment(const codepoint&);
    };

    class lexer {
      private:
        std::unordered_map<unsigned, automaton> automata;
        tracker locationManager;
        std::queue<codepoint> memory;
      public:
        lexer(void) = default;
        explicit lexer(std::unordered_map<unsigned, automaton> ) noexcept;
        lexer(
            std::initializer_list<std::pair<const unsigned, automaton>>
        ) noexcept;
        lexer(const lexer&) = default;
        lexer(lexer&&) noexcept = default;
        ~lexer(void) = default;
        lexer& operator=(const lexer&) = default;
        lexer& operator=(lexer&&) noexcept = default;
        token* lex(scanner&);
      private:
        codepoint peek(scanner&) noexcept;
        codepoint pop(scanner&, const bool&) noexcept;
    };
}

#endif