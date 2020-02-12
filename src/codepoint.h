#ifndef PARSER_FRONTEND_CODEPOINT_H
#define PARSER_FRONTEND_CODEPOINT_H

#include <cstdint>
#include <string>

namespace enjoy {
    typedef uint_fast32_t codepoint;

    std::string toString(const codepoint&) noexcept;
}

#endif