#ifndef PARSER_FRONTEND_SCANNER_H
#define PARSER_FRONTEND_SCANNER_H

#include <cstdint>
#include <functional>
#include <istream>
#include <queue>

#include "codepoint.h"

namespace enjoy {
    class scanner {
      private:
        typedef uint_fast8_t byte;
        typedef uint_fast16_t codeunit;
        enum encoding {
            utf8,
            utf16be,
            utf16le,
            utf32be,
            utf32le,
        };
        encoding encoded;
        std::queue<byte> memory;
        std::istream* stream;
      public:
        scanner(void) noexcept;
        explicit scanner(std::istream*) noexcept;
        scanner(const scanner&) noexcept;
        scanner(scanner&&) noexcept;
        ~scanner(void) = default;
        scanner& operator=(const scanner&) noexcept;
        scanner& operator=(scanner&&) noexcept;
        std::function<codepoint(void)> scan;
      private:
        void detect(void) noexcept;
        void setup(void) noexcept;
        byte readStream(void) noexcept;
        byte getNextByte(void) noexcept;
        codeunit getCodeunit(const bool&) noexcept;
        codepoint getUtf8(void) noexcept;
        codepoint getUtf16(const bool&) noexcept;
        codepoint getUtf32(const bool&) noexcept;
    };
}

#endif